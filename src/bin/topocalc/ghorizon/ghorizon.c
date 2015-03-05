/*
** NAME
** 	ghorizon -- create mask for sun below horizon
** 
** SYNOPSIS
**	void ghorizon (fdi, fds, fdm, fdo, got_solar, azm, maskout, zenith,
**		       emax, blat, blon, dlat, dlon);
** 	int fdi, fds, fdm, fdo;
**	bool_t got_solar;
**	double azm;
**	bool_t maskout;
**	bool_t zenith;
**	float emax;
**	double blat, blon, dlat, dlon;
** 
** DESCRIPTION
** 	ghorizon reads in the elevation and solar image (if provided), computes
**	the horizon in the direction of the given azimuth (or solar azimuth
**	if got_solar) for each pixel and writes the output image: cosine
**	horizon angle from vertical or, if mask output desired, a mask image
**	with 0 output for solar zenith angle below the calculated horizon and
**	1 if the sun is above the horizon.
** 
** GLOBALS ACCESSED
**	globals accessed from pgm.h:
**		coslon, sinlon, coslat, sinlat, cosazm, sinazm,
**		cosdist, sindist
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "rearth.h"
#include "pgm.h"

void
ghorizon(
        int             fdi,		/* input file desc		 */
	int             fds,		/* solar file desc		 */
	int             fdm,		/* mask file desc		 */
	int             fdo,		/* output file desc		 */
	bool_t		got_solar,	/* flag for solar image given	 */
	double		azm,		/* azimuth in deg S		 */
	bool_t		maskout,	/* flag for mask image output	 */
	bool_t		zenith,		/* flag for zenith ang in solar	 */
	float		emax,		/* maximum elevation in map	 */
	double		blat,		/* lat of image UL corner (deg)	 */
	double		blon,		/* lon of image UL corner (deg)	 */
	double		dlat,		/* latitude pixel spacing (deg)	 */
	double		dlon)		/* longitude pixel spacing (deg) */
{
	int		nsamps;		/* # samples in input image	 */
	int		nlines;		/* # lines in input image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	int		max_pts;	/* max #pts to search for horizon*/
	int		max_pts_curv;	/* max #pts based on Earth curv	 */
	int		maskv;		/* mask output from horizon()	 */
	int		s_nbands = 0;	/* # bands in solar file	 */
	pixel_t        *sbuf;		/* -> solar image buffer	 */
	pixel_t	       *mbuf;		/* -> mask image buffer		 */
	pixel_t        *sbufp;		/* -> in solar image buffer	 */
	pixel_t	       *mbufp;		/* -> in mask image buffer	 */
	fpixel_t      **ibuf;		/* -> input buffer		 */
	fpixel_t       *obuf;		/* -> output buffer		 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	fpixel_t       *fpmax;		/* -> F.P. max of input image	 */
	float	       *tzentbl;	/* -> table of tan zenith 	 */
	double		max_dist;	/* max distance to horizon	 */
	float		tanzen = 0.0;	/* tangent of zenith angle	 */
	float		hcos;		/* cosine horizon angle		 */
	float	       *acostbl;	/* -> table of cosine azimuth	 */
	float	       *asintbl;	/* -> table of sine azimuth	 */
	double		lon;		/* current longitude (radians)	 */
	double		lat;		/* current latitude (radians)	 */
	double		blat_r;		/* image UL lat in radians	 */
	double		blon_r;		/* image UL lon in radians	 */
	double		dlat_r;		/* delta lat in radians		 */
	double		dlon_r;		/* delta lon in radians		 */
	double		cosblat;	/* cos of blat			 */
	double		sinblat;	/* sin of blat			 */
	double		azmr;		/* azimuth in radians from N	 */
	double		dist;		/* distance			 */
	double		step_size;	/* GCP step size (meters)	 */
	double		step_size_r;	/* GCP step size (radians)	 */


	nsamps = hnsamps(fdi);
	nlines = hnlines(fdi);

   /* convert latitudes and longitudes to radians */
   /* longitudes are -pi to pi */
   /* latitudes are -pi (S. Pole) to pi (N. Pole) */

	blat_r = blat * PIO180;
	blon_r = blon * PIO180;
	dlat_r = dlat * PIO180;
	dlon_r = dlon * PIO180;

   /* convert azimuth from 0 deg at S. Pole to 0 at N. Pole (-W/+E) */
   /* compute sin and cos of given azimuth */

	if (!got_solar) {
		azm = 180.0 - azm;
		if (azm > 180.0)
			azm -= 360.0;
		azmr = azm * PIO180;
		sinazm = sin (azmr);
		cosazm = cos (azmr);
	}


   /* calculate maximum range to search for horizon:
    *    maximum range is where the Earth curvature correction is equal to
    *    the maximum elevation in the input image
    *	 Note: within this maximum range no elevations are corrected for
    *    Earth curvature
    */

	if (emax <= 0.0) {
		fpmax = fpfmax (fdi);
		emax = fpmax[0];
	}
	max_dist = (double) ( RAD_EARTH * acos (RAD_EARTH / (RAD_EARTH + (double) emax)) );

   /* set great circle step size to 1/2 of the width of a Northernmost cell */

	sinblat = sin (blat_r);
	cosblat = cos (blat_r);
	gcp_dist (sinblat, cosblat, blon_r, sinblat, cosblat, blon_r+dlon_r,
                  &dist, &azmr);
	step_size = 0.5 * dist;
	max_pts = max_pts_curv =  max_dist / step_size;
	step_size_r = step_size / RAD_EARTH;

   /* allocate input and output buffers */

	if (got_solar)
		s_nbands = hnbands(fds);
   /*
    *   iobuf (nlines, nsamps, s_nbands, got_solar, fdm,
    *	       &ibuf, &sbuf, &mbuf, &obuf);
    */


   /* allocate input buffer - need to read in the entire file */

	ibuf = (fpixel_t **) allocnd (sizeof(fpixel_t), 2, nlines, nsamps);
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* allocate solar buffer */

	if (got_solar) {
		sbuf = (pixel_t *) ecalloc (s_nbands * nsamps, sizeof(pixel_t));
		if (sbuf == NULL) {
			error ("can't allocate solar image buffer");
		}
	} else
		sbuf = NULL;

   /* allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask image buffer");
		}
	} else
		mbuf = NULL;

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

   /* allocate buffers to hold sines and cosines of solar azimuth angles, */
   /* buffer to hold tangents of zenith angles, buffers to hold sines and */
   /* cosines of all latitudes and longitudes and all distance increments */

	trigbuf (nlines, nsamps, got_solar, maskout, max_pts_curv, fds,
 		 &acostbl, &asintbl, &tzentbl,
		 &coslon, &sinlon, &cosdist, &sindist);

   /* initialize trig/zenith lookup tables */

	trigtbl (fds, got_solar, maskout, asintbl, acostbl, tzentbl, zenith,
		 sinlon, coslon, blon_r, dlon_r, nsamps,
		 sindist, cosdist, max_pts_curv, step_size_r);

   /* read entire elevation file */

	for (line = 0; line < nlines; line++) {
		if (fpvread (fdi, ibuf[line], nsamps) != nsamps) {
			error ("error reading elevation image - line %d", line);
		}
	}

	lon = blon_r;
	lat = blat_r;

   /* loop reading input lines, calculating cosines and writing to output */

	for (line = 0; line < nlines; line++) {

		/* read solar image line */

		if (got_solar) {
			if (pvread (fds, sbuf, nsamps) != nsamps) {
				error ("error reading solar image, line %d", line);
			}
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image, line %d", line);
			}
		}

		sinlat = sin (lat);
		coslat = cos (lat);

		/* perform calculation for each sample */

		sbufp = sbuf;
		obufp = obuf;
		mbufp = mbuf;
		for (samp = 0; samp < nsamps; samp++) {

			if (maskout) {
				tanzen = tzentbl[*sbufp++];
				if (tanzen > 0) {
					max_dist = emax / tanzen;
					max_pts =  MIN (max_dist / step_size + 1,
							max_pts_curv);
				}
			}

			if (fdm == ERROR || *mbufp) {

				if (got_solar) {
					sinazm = asintbl[*sbufp];
					cosazm = acostbl[*sbufp];
				}

				/* for mask output - */
				/* if zenith <= 0 (below horizon) nothing to do */

				if (maskout && (tanzen <= 0.0)) {
					maskv = 0;

				/* compute cosine horizon angle or mask value */

				} else {
					horizon (line, samp, ibuf, nlines, nsamps,
					 	max_pts, lon,
					 	blat_r, blon_r, dlat_r, dlon_r, 
					 	tanzen, maskout, step_size, emax,
					 	&hcos, &maskv);
				}

				if (maskout) {
					*obufp = maskv;
				} else {
					*obufp = hcos;
				}
			} else {
				*obufp = 0;
			}
			sbufp++;
			mbufp++;
			obufp++;

			lon += dlon_r;
		}

		/* write output buffer */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("error writing output buffer, line %d", line);
		}

		lon = blon_r;
		lat += dlat_r;
	}
}
