/*
** NAME
** 	headers -- read/write headers for ggradient program
** 
** SYNOPSIS
**	headers (fdi, fdm, fdo, slope, aspect, nbits, blat, blon, dlat, dlon);
** 	int fdi, fdm, fdo;
**	bool_t slope, aspect;
**	int *nbits;
**	double *blat, *blon, *dlat, *dlon;
** 
** DESCRIPTION
** 	Headers reads/processed the headers of the input images and writes
**	the headers for the output image of the ggradient program.  The
**	latitude and longitude of the upper left corner of the image and
**	the latitude and longitude spacing in degrees are returned.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "lqh.h"
#include "geoh.h"
#include "gethdrs.h"
#include "pgm.h"

void
headers(
	int             fdi,		/* input image file descriptor	 */
	int             fdm,		/* mask image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	bool_t		slope,		/* flag to compute slope	 */
	bool_t		aspect,		/* flag to compute aspect	 */
	int	       *nbits,		/* # bits in output image	 */
	double	       *blat,		/* Northernmost lat (deg)	 */
	double	       *blon,		/* Easternmost long (deg)	 */
	double	       *dlat,		/* latitude spacing (deg)	 */
	double	       *dlon)		/* longitude spacing (deg)	 */
{
	int		nlines;		/* # lines in input images	 */
	int		nsamps;		/* # samples in input images	 */
	int		obands;		/* # bands in output image	 */
	int		j;		/* loop counter			 */
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **m_bih;		/* mask BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	GEOH_T        **i_geoh;		/* input GEOH			 */
	GEOH_T        **o_geoh;		/* output GEOH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	double		elat;		/* lat of image LR corner	 */
	double		elon;		/* lon of image LR corner	 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_lqh, &h_geo, 0};



   /* read BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read BIH of input image");
	}

   /* must be single band in input elevation file */

	if (hnbands(fdi) != 1) {
		error ("input elevation file must have one band");
	}

   /* make sure input image has spacing */

	nlines = bih_nlines (i_bih[0]);
	nsamps = bih_nsamps (i_bih[0]);
	if (nlines < 2) {
		error ("only %ld line in input image", bih_nlines(i_bih[0]));
	}

	if (nsamps < 2) {
		error ("only %ld samp in input image", bih_nsamps(i_bih[0]));
	}

   /* read mask BIH */
   /* make sure it has same dimensions of input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (nsamps != bih_nsamps(m_bih[0]) ||
   	    	    nlines != bih_nlines(m_bih[0])) {
			error ("input and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* create output BIH, # bytes will be set by bihmake */

	if (slope && aspect) {
		obands = 2;
	} else {
		obands = 1;
	}

	o_bih = (BIH_T **) hdralloc (obands, sizeof(BIH_T *), fdo, BIH_HNAME);
	for (j = 0; j < obands; j++) {
		o_bih[j] = bihmake (0, nbits[j],
				   bih_history(i_bih[0]), bih_annot(i_bih[0]),
				   o_bih[0],
				   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
				   obands);
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest LQH and GEOH; other headers get passed thru */

	gethdrs (fdi, request, obands, fdo);

   /* read geo header - make sure image has lat/lon projection */
   /* copy geo header to output image */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geo)) != NULL) {
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

		o_geoh = (GEOH_T **) hdralloc (obands, sizeof(GEOH_T *),
					      fdo, GEOH_HNAME);
		o_geoh[0] = i_geoh[0];
		if (obands > 1)
			o_geoh[1] = i_geoh[0];
		if (geohwrite(fdo, o_geoh) == ERROR) {
			error ("Can't write output GEOH");
		}
	} else {
		error ("input image has no geodetic header - must be lat/lon projection");
	}

   /* check that we read LQH */

	if (hdr_addr(h_lqh) == NULL) {
		warn ("input file has no LQH - raw values used");
	}

   /* create and write new LQH */

	if ((o_lqh = newlqh (fdo, slope, aspect)) == NULL) {
		error ("Can't create new LQH");
	}

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* get ready for rest of output */

	if (boimage (fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
