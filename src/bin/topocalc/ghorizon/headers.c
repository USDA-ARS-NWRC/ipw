/*
** NAME
** 	headers -- read/write headers for ghorizon program
** 
** SYNOPSIS
**	headers (fdi, fds, fdm, fdo, got_solar, azm, maskout, zenith,
**		 blat, blon, dlat, dlon);
** 	int fdi, fds, fdm, fdo;
**	bool_t got_solar;
**	double azm;
**	bool_t maskout;
**	bool_t zenith;
**	double *blat, *blon, *dlat, *dlon;
** 
** DESCRIPTION
** 	Headers reads/processed the headers of the input images and writes
**	the headers for the output image of the ghorizon program.  The
**	latitude and longitude of the upper left corner of the image and
**	the latitude and longitude spacing in degrees are returned.
**
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "lqh.h"
#include "horh.h"
#include "geoh.h"
#include "gethdrs.h"
#include "pgm.h"

void
headers(
	int             fdi,		/* input image file descriptor	 */
	int             fds,		/* solar image file descriptor	 */
	int             fdm,		/* mask image file descriptor	 */
	int             fdo,		/* output image file descriptor	 */
	bool_t		got_solar,	/* flag if solar image included	 */
	double		azm,		/* azimuth (if not got_solar)	 */
	bool_t		maskout,	/* flag for mask output image	 */
	bool_t		zenith,		/* flag if fds contains zenith	 */
	double	       *blat,		/* lat of image UL corner	 */
	double	       *blon,		/* lon of image UL corner	 */
	double	       *dlat,		/* latitude spacing (deg)	 */
	double	       *dlon)		/* longitude spacing (deg)	 */
{
	int		nlines;		/* # lines in input images	 */
	int		nsamps;		/* # samples in input images	 */
	int		nbits;		/* # bits in output image	 */
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **s_bih;		/* solar BIH			 */
	BIH_T         **m_bih;		/* mask BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	GEOH_T        **i_geoh;		/* input GEOH			 */
	GEOH_T        **o_geoh;		/* output GEOH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	HORH_T	      **o_horh;		/* output HORH			 */
	fpixel_t       *fpmin;		/* min value in F.P. map	 */
	fpixel_t       *fpmax;		/* max value in F.P. map	 */
	double		elat;		/* lat of image LR corner	 */
	double		elon;		/* lon of image LR corner	 */
	double		azmr;		/* azimuth in radians		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T sh_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, &h_geo, 0};
	static GETHDR_T *srequest[] = {&sh_lqh, 0};


   /* read BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read BIH of input image");
	}

   /* must be single bands in input elevation file */

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

   /* read solar BIH */

	if (got_solar) {
		if ((s_bih = bihread(fds)) == NULL) {
			error ("can't read BIH of solar image");
		}

		/* check #bands in solar image */

		if (maskout) {
			if (hnbands(fds) != 2)
				error ("input solar image must have 2 bands");
		} else {
			if (hnbands(fds) != 1)
				error ("input solar image must have one band");
		}

		/* make sure input and solar images have same dimensions */

		if (nsamps != bih_nsamps(s_bih[0]) ||
		    nlines != bih_nlines(s_bih[0])) {
			error ("input and solar images have different dimensions");
		}
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

   /* create output BIH */

	o_bih = (BIH_T **) hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL) {
		error ("Can't make new BIH");
	}

	if (maskout)
		nbits = 1;
	else
		nbits = CHAR_BIT;

	if ((o_bih[0] = bihmake (0, nbits,
				 bih_history(i_bih[0]), bih_annot(i_bih[0]),
				 (BIH_T *) NULL, nlines, nsamps, OBANDS))
			== NULL) {
		error ("Can't make new BIH");
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest LQH and GEOH; other headers get passed thru */

	gethdrs (fdi, request, OBANDS, fdo);
	if (got_solar)
		gethdrs (fds, srequest, NO_COPY, ERROR);

   /* create and write new LQH for cos horizon angle output only */

	if (!maskout) {
		if ((o_lqh = newlqh (fdo)) == NULL) {
			error ("Can't create new LQH");
		}

		if (lqhwrite (fdo, o_lqh) == ERROR) {
			error ("Can't write output LQH");
		}
	}

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

		o_geoh = (GEOH_T **) hdralloc (OBANDS, sizeof(GEOH_T *),
					      fdo, GEOH_HNAME);
		o_geoh[0] = i_geoh[0];
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

   /* check solar image */

	if (got_solar) {
		if (hdr_addr(sh_lqh) == NULL) {
			error ("solar file has no LQH");
		}

		fpmin = fpfmin (fds);
		fpmax = fpfmax (fds);

		if (maskout) {
			if (zenith) {
				if (fpmin[0] < -180.0 || fpmax[0] > 180.0)
					error ("band 0 of solar image not zenith");
			} else {
				if (fpmin[0] < -1.0 || fpmax[0] > 1.0)
					error ("band 0 of solar image not cos zenith angle");
			}
			if (fpmin[1] < -180.0 || fpmax[1] > 180.0)
				error ("band 1 of solar image not azimuth");
		} else {
			if (fpmin[0] < -180.0 || fpmax[0] > 180.0)
				error ("band 0 of solar image not azimuth");
		}

   /* single azimuth for horizons - output HORH (for viewcalc) */

	} else {

		o_horh = (HORH_T **) hdralloc (1, sizeof(HORH_T *), fdo,
					       HORH_HNAME);
		if (o_horh == NULL) {
			error ("Can't allocate HORH");
		}
		azmr = azmf (azm);
		o_horh[0] = horhmake (azmr);
		if (o_horh[0] == NULL) {
			error ("Can't create HORH");
		}

		/* write output HORH */

		if (horhwrite (fdo, o_horh) == ERROR) {
			error ("can't write output HORH");
		}
	}

   /* get ready for rest of output */

	if (boimage (fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
