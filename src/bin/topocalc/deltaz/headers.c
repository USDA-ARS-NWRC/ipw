/*
** NAME
** 	headers  -- read input header/write output images headers for surface
** 
** SYNOPSIS
**	void
**	headers (fdm, fdo, min, max, bline, bsamp, dline, dsamp, bits)
**	int fdm, fdo, bits;
**	float min, max;
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

void
headers (
	int             fdm,		/* DEM/mask file descriptor	 */
	int             fdo,		/* output image file descriptors */
	float		min,		/* min value of point data	 */
	float		max,		/* max value of point data	 */
	double	       *bline,		/* line-coord of UL image corner */
	double	       *bsamp,		/* samp-coord of UL image corner */
	double	       *dline,		/* line spacing			 */
	double	       *dsamp,		/* sample spacing		 */
	int		bits)		/* output bits  		 */
{
	BIH_T         **i_bih;		/* -> BIH of input image	 */
	LQH_T         **i_lqh;		/* -> LQH of input image	 */
	GEOH_T        **i_geoh;		/* -> GEOH of input image	 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	LQH_T         **o_lqh;		/* -> output LQH	 	 */
	GEOH_T        **o_geoh;		/* -> output GEOH	 	 */
	pixel_t		ibkpt[2];	/* integer break points		 */
	fpixel_t	fbkpt[2];	/* FP break points		 */
	fpixel_t       *demmap;         /* dem lq map			 */
	int		demlast; 	/* index to end of demmap  	 */

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, &h_geoh, 0};


   /* read DEM/mask headers */

	if ((i_bih = bihread(fdm)) == NULL) {
		error ("can't read BIH of mask image");
	}

   /* check for second band */

	nbands = bih_nbands (i_bih[0]);
	if (nbands != 1 && nbands != 2) {
		error ("dem image may only have 1 or 2 bands");
	}

   /* create output BIH, # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);

	o_bih[0] = bihmake (0, bits,
			    bih_history(i_bih[0]), bih_annot(i_bih[0]),
			    o_bih[0], bih_nlines(i_bih[0]),
			    bih_nsamps(i_bih[0]), 1);

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest input LQH and GEOH; other headers are passed through */

	gethdrs (fdm, request, 1, fdo);

   /* make sure input header has geo header */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) == NULL) {
		error ("input header has no geodetic header");
	}

	*bline = geoh_bline (i_geoh[0]);
	*bsamp = geoh_bsamp (i_geoh[0]);
	*dline = geoh_dline (i_geoh[0]);
	*dsamp = geoh_dsamp (i_geoh[0]);

   /* Make sure projection is not lat/lon */

	if (streqn(geoh_units(i_geoh[0]), "deg", 3)) {
		error ("input geo units are degrees - must be Euclidean units");
	}

   /* check for increasing northings */

	if (*dline > 0.0) {
 		warn ("dline > 0 in GEO header - northings should DECREASE");
	}

   /* create and write geo header for output image */

	o_geoh = geohdup (i_geoh, 1);

	if (geohwrite (fdo, o_geoh) == ERROR) {
		error ("Can't write GEOH to output image");
	}


   /* make LQH for output image */

	/* get min/max from the dem LQH */

	if ((i_lqh = (LQH_T **) hdr_addr (h_lqh)) != NULL) {
		demmap = lqh_map(i_lqh[0]);
        } else {
       		error ("dem must have an LQ header");
	} 

	/* allocate output LQH */

	if ((o_lqh = (LQH_T **) hdralloc (1, sizeof(LQH_T *), fdo,
					     LQH_HNAME)) == NULL) {
		error ("can't allocate LQH header");
	}

	/* create output LQH */

	ibkpt[0] = 0;
	ibkpt[1] = ipow2(bits) - 1;
	demlast  = ipow2(bih_nbits(i_bih[0])) - 1;        

        /* output min/max are based on extreme values of dem and station z */

	fbkpt[0] = demmap[0] - max;
	fbkpt[1] = demmap[demlast] - min;

fprintf(stderr,"NOTE: dem extrema = %.2f, %.2f \n", demmap[0], demmap[demlast]);
fprintf(stderr,"NOTE: lq extrema  = %.2f, %.2f \n", fbkpt[0],fbkpt[1]);

	if ((o_lqh[0] = lqhmake (bits, 2,
		ibkpt, fbkpt, (char *) NULL, (char *) NULL)) == NULL) {
		error ("can't make LQH");
	}

   /* write LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for image output */

	if (boimage(fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
