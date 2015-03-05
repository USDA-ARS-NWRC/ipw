/*
** NAME
** 	headers  -- read input header/write output images headers for surface
** 
** SYNOPSIS
**	void
**	headers (fdi, fdm, fdo, bline, bsamp, dline, dsamp)
**	int fdi, fdm, fdo;
**	double *bline, *bsamp, *dline, *dsamp;
** 
** DESCRIPTION
**	Headers reads/ingests/checks the input header and mask image
**	headers	and creates/writes the headers of the output image for the
**	program	surface.
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "pgm.h"
#include <math.h>

void
headers (
	int             fdi,		/* header file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output image file descriptors */
	double	       *bline,		/* line-coord of UL image corner */
	double	       *bsamp,		/* samp-coord of UL image corner */
	double	       *dline,		/* line spacing			 */
	double	       *dsamp,		/* sample spacing		 */
	int             nneibs,		/* # columns in output image     */
	FILE           *ptfile,         /* ascii point file              */
	int            *npoints)        /* # points in ptfile            */
{
	int		nbands;		/* # bands in images		 */
	int		nlines;		/* # lines in images		 */
	int		nsamps;		/* # samps in images		 */
	int		band;		/* band loop counter		 */
	int		i;		/* loop counter		 	 */
	double	        cell;		/* coord counter		 */
	int		Read_points();  /* function to read pt file	 */
	BIH_T         **i_bih;		/* -> BIH of input image	 */
	GEOH_T        **i_geoh;		/* -> GEOH of input image	 */
	BIH_T         **m_bih;		/* -> mask BIH			 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	GEOH_T        **o_geoh;		/* -> output GEOH	 	 */
        STRVEC_T       *annotp;         /* -> annotation string vector   */

/***	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, &h_geoh, 0};
***/
	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
    	static GETHDR_T h_lqh = {LQH_HNAME};
	static GETHDR_T *request[] = {&h_geoh, &h_lqh, 0};


   /* read input header BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* single-band images only */

	nlines = bih_nlines(i_bih[0]);
	nsamps = bih_nsamps(i_bih[0]);
	nbands = bih_nbands (i_bih[0]);
	if (nbands > 1) {
		error ("single-band input images only");
	}

   /* read mask headers */
   /* make sure it has same dimensions as input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]) ||
       	    	    bih_nlines(i_bih[0]) != bih_nlines(m_bih[0])) {
		      error ("input and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* get coordinates from point file for annotation strings */

  	/* create 1st annotation string */

    	annotp = addsv(NULL, 
		 "These points (east,north) were used to make this image: ");

	*npoints = Read_points(ptfile, nneibs, annotp);

/***
for(band = 1; band <= *npoints; band++) 
  fprintf(stderr,"annot[%d] = %s\n", band, annotp->v[band]);
fprintf(stderr,"headers debug: npoints = %d\n", *npoints);
***/

   /* create output BIH, # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (nneibs, sizeof(BIH_T *), fdo, BIH_HNAME);

        /* put points as annotation strings in output band 0 only */	

	o_bih[0] = bihmake (BYTES, BITS,
			    bih_history(i_bih[0]), annotp,
			    o_bih[0], 
			    bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			    nneibs);

 	/* for remaining bands, no annot strings; no early history strings */

	for(band = 1; band < nneibs; band++) {
	    o_bih[band] = bihmake (BYTES, BITS,
			    (STRVEC_T *) NULL, (STRVEC_T *) NULL,
			    o_bih[0], 
			    bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			    nneibs);
	}


   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest input LQH and GEOH; other headers are passed through */

	gethdrs (fdi, request, nbands, fdo);

   /* make sure input header has geo header */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) == NULL) {
		error ("input header has no geodetic header");
	}

	*bline = geoh_bline (i_geoh[0]);
	*bsamp = geoh_bsamp (i_geoh[0]);
	*dline = geoh_dline (i_geoh[0]);
	*dsamp = geoh_dsamp (i_geoh[0]);

   /* warn if -g not used with "deg" units */
 
        if (streqn(geoh_units(i_geoh[0]), "deg", 3) && !parm.geoflag) {
            warn("Using Euclidean distance with 'deg' units");
        }
 
   /* warn if -g used with big-numbered coords */
 
        if(parm.geoflag &&
          ( (*bline < -90.0 || *bline > 90.0) ||
            (*bsamp < -180.0 || *bsamp > 180))   ) {
                warn ("Using geo distance with seemingly non-geo units.");
        }

   /* if -g, make arrays of sin/cos for lines/samps */

        if(parm.geoflag) { 
	
	    linetrig   = (TRIG *)   ecalloc(nlines, sizeof(TRIG));
	    samplonrad = (double *) ecalloc(nsamps, sizeof(TRIG));

	    for(i=0, cell = *bline; i<nlines; i++, cell+=*dline){
		linetrig[i].sin = sin(cell/DEG);
		linetrig[i].cos = cos(cell/DEG);
	    }

	    for(i=0, cell = *bsamp; i<nsamps; i++, cell+=*dsamp){
		samplonrad[i]= cell/DEG;
	    }

	}
 
   /* check for increasing northings */

	if (*dline > 0.0) {
 	    warn ("geo hdr: dline is negative - northings should DECREASE");
	}

   /* create and write geo header for output image */

    o_geoh = (GEOH_T **) hdralloc (nneibs, sizeof(GEOH_T *), fdo, GEOH_HNAME);

	for(band = 0; band < nneibs; band++) {
	    o_geoh[band] = i_geoh[0];
	}


	if (geohwrite (fdo, o_geoh) == ERROR) {
		error ("Can't write GEOH to output image");
	}


   /* get ready for image output */

	if (boimage(fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
