/*
** NAME
** 	headers  -- read input header/write output images headers for gsunlight
** 
** SYNOPSIS
**	void
**	headers (fdg, fdm, fdo, blat, blon, dlat, dlon)
**	int fdg, fdm, fdo;
**	double *blat, *blon, *dlat, *dlon;
** 
** DESCRIPTION
**	Headers reads/ingests/checks the input geodetic header and mask image
**	headers	and creates/writes the headers of the output image for the
**	program	gsunlight.
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "orh.h"
#include "fpio.h"
#include "pgm.h"

static int nbits[2] = {16, 16};

void
headers (
	int             fdg,		/* input lat/lon file descriptor */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output image file descriptors */
	double	       *blat,		/* UL latitude from input geoh	 */
	double	       *blon,		/* UL longitude from input geoh	 */
	double	       *dlat,		/* LR latitude from input geoh	 */
	double	       *dlon)		/* LR longitude from input geoh	 */
{
	int		nlines;		/* # lines in output image	 */
	int		nsamps;		/* # samples in output image	 */
	BIH_T         **i_bih;		/* -> BIH of geoh file		 */
	static GEOH_T **i_geoh;		/* -> input GEOH		 */
	BIH_T         **m_bih;		/* -> mask BIH			 */
	static BIH_T  **o_bih;		/* -> output BIH		 */
	static LQH_T  **o_lqh;		/* -> output LQH	 	 */
	static GEOH_T **o_geoh;		/* -> output GEOH	 	 */
	int             j;		/* counter			 */
	int		band;		/* band loop counter		 */
	double		elat;		/* LR latitude from input geoh	 */
	double		elon;		/* LR longitude from input geoh	 */

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_geoh, NULL};



   /* read BIH of input geoh file */

	if ((i_bih = bihread(fdg)) == NULL) {
		error ("can't read input BIH");
	}

	nlines = bih_nlines (i_bih[0]);
	nsamps = bih_nsamps (i_bih[0]);

   /* read mask BIH */
   /* make sure it has same dimensions of input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (nsamps != bih_nsamps(m_bih[0]) ||
       	    	    nlines != bih_nlines(m_bih[0])) {
			error ("input and mask have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* create output BIH, # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (OBANDS, sizeof(BIH_T *), fdo,
					     BIH_HNAME);
	for (j = 0; j < OBANDS; ++j) {
		o_bih[j] = bihmake (0, nbits[j],
			   	bih_history(i_bih[0]), bih_annot(i_bih[0]),
			   	o_bih[0], nlines, nsamps, OBANDS);
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest GEOH; other headers are skipped */

	gethdrs (fdg, request, NO_COPY, ERROR);

   /* read geo header - make sure image has lat/lon projection */
   /* copy geo header to output image */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) != NULL) {
		*blat = geoh_bline (i_geoh[0]);
		*dlat = geoh_dline (i_geoh[0]);
		elat = GEO_LINE (i_geoh[0], nlines-1);
		if (*dlat >= 0.0 || *blat > 90.0 || elat < -90.0) {
			error ("Illegal latitudes in GEO header");
		}

		*blon = geoh_bsamp (i_geoh[0]);
		*dlon = geoh_dsamp (i_geoh[0]);
		elon = GEO_SAMP (i_geoh[0], nsamps-1);
		if (*dlon <= 0.0 || *blon < -180.0 || elon > 180.0) {
			error ("Illegal longitudes in GEO header");
		}

		o_geoh = (GEOH_T **) hdralloc (OBANDS, sizeof(GEOH_T *),
					      fdo, GEOH_HNAME);
		for (band = 0; band < OBANDS; band++) {
			o_geoh[band] = i_geoh[0];
		}
		if (geohwrite(fdo, o_geoh) == ERROR) {
			error ("Can't write GEOH for output image");
		}
	} else {
		error ("input image has no geodetic header - must be lat/lon projection");
	}

   /* create new LQH */

	o_lqh = newlqh (fdo);
	if (o_lqh == NULL) {
		error ("Can't create new LQH");
	}

   /* write output LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}
		
   /* get ready for rest of output */

	if (boimage(fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
