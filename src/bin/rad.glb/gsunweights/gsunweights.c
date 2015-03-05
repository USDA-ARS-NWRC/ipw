/*
** NAME
** 	sunweights -- calculate sun angles and Kronrod weights over an image
** 
** SYNOPSIS
**	void sunweights (fdm, prefix, kt, year, month, day, nlines, nsamps, nbits,
**   			 blat, blon, dlat, dlon)
**	int fdm;
**	char *prefix;
**	int kt;
**	int year, month, day;
**	int nlines, nsamps, *nbits;
**	double blat, blon, dlat, dlon;
** 
** DESCRIPTION
** 	Sunweights calculates sun angles (cos zenith angle and azimuth) and
**	Kronrod weights for each pixel in the output images.
** 
*/

#include <time.h>
#include <math.h>
#include "ipw.h"
#include "pixio.h"
#include "pgm.h"
#include "solar.h"
#include "kron.h"

void
sunweights (
	int		fdm,		/* mask image file descriptor	 */
	char	       *prefix,		/* output image name prefix	 */
	int		kt,		/* # Kronrod quadrature points	 */
	int		year,		/* year 			 */
	int		month,		/* month (1-12)			 */
	int		day,		/* day (1-31)			 */
	int		nlines,		/* # lines in output image	 */
	int		nsamps,		/* # samps in output image	 */
	int	       *nbits,		/* # bits/band in output image	 */
	double		blat,		/* UL latitude of image (deg)	 */
	double		blon,		/* UL longitude of image (deg)	 */
	double		dlat,		/* latitude pixel spacing (deg)	 */
	double		dlon)		/* longitude pixel spacing (deg) */
{
	int		line;		/* line loop counter		 */
	int		samp;		/* samp loop counter		 */
	int		npixels;	/* # pixels in image		 */
	int		ikt;		/* kronrod point loop counter	 */
	int		fdt;		/* temporary file descriptor	 */
	int		onbytes;	/* # bytes in output buffer	 */
	pixel_t        *mbuf =NULL;	/* -> mask line buffer		 */
	pixel_t        *mbufp;		/* -> in mask line buffer	 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	datetime_t     *dt_rise;	/* sunrise time			 */
	datetime_t     *dt_set;		/* sunset time			 */
	int		daylen_day;	/* day length (days component)   */
	int		daylen_sec;	/* day length (seconds component)*/
	double          lat;		/* latitude (radians)	 	 */
	double          lon;		/* longitude (radians)	 	 */
	double		dlat_r;		/* latitude spacing (radians)	 */
	double		dlon_r;		/* longitude spacing (radians)	 */
	double		delta_t;	/* time difference in seconds	 */
	double		xg =0.0;	/* kronrod point abscissa coeff	 */
	double		wg =0.0;	/* kronrod point weight coeff	 */
	double		wt;		/* kronrod weight		 */
	datetime_t      dt;		/* date-time for Kronrod pt 	 */
	double		cosz;		/* cosine solar zenith angle	 */
	double		azm;		/* solar azimuth		 */
	double		mins[OBANDS];	/* min value of each band	 */
	double		maxs[OBANDS];	/* max value of each band	 */
	datetime_t     *centr;		/* center-of-day buffer		 */
	double	       *hlen;		/* half-day-length buffer	 */
	datetime_t     *centrp;		/* -> in centr buffer		 */
	double	       *hlenp;		/* -> in hlen buffer		 */
	char	       *tname;		/* name of temporary file	 */


   /* convert lat/lon to radians */

	lat    = DEG_TO_RAD(blat);
	lon    = DEG_TO_RAD(blon);
	dlat_r = DEG_TO_RAD(dlat);
	dlon_r = DEG_TO_RAD(dlon);

   /* allocate mask buffer for entire file */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps * nlines, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("error allocating mask buffer");
		}
	}

   /* allocate buffers for center-of-day and half-day-length */

	centr = (datetime_t *) ecalloc (nlines, sizeof(datetime_t));
	if (centr == NULL) {
		error ("error allocating centr buffer");
	}

	hlen = (double *) ecalloc (nlines, sizeof(double));
	if (hlen == NULL) {
		error ("error allocating hlen buffer");
	}

   /* allocate output line buffer */

	onbytes = nsamps * OBANDS * sizeof(float);
	obuf = (float *) ecalloc (onbytes, 1);
	if (obuf == NULL) {
		error ("error allocating output buffer");
	}

   /* get name for temporary file */

	tname = mktemplate("sunweights");
	if (tname == NULL) {
		error ("Can't get name for temporary file");
	}

   /* delta_t is the time difference (in seconds) between sunrises of */
   /* consecutive pixels on a line (since they have the same latitude) */

	delta_t = -dlon * SECS_IN_DAY / 360.0;

	centrp = centr;
	hlenp = hlen;

   /* loop on image lines computing sunrise and sunset, and */
   /* saving center-of-day and half-day-length */

	for (line = 0; line < nlines; line++) {

		/* compute sunrise, sunset and center-of-day time */
		/* for first pixel in line */
#if 0
		sunrise (lat, lon, year, month, day, &trise);
		sunset (lat, lon, year, month, day, &tset);
#endif
		dt_rise = sunrise(lat, lon, year, month, day);
		dt_set  = sunset (lat, lon, year, month, day);

		/* day length is the same for all pixels of constant latitude */
#if 0
		*hlenp++ = (tset - trise) * 0.5;
#endif
		dt_diff(dt_set, dt_rise, &daylen_day, &daylen_sec);
		*hlenp = (DAY_TO_SEC(daylen_day) + daylen_sec) * 0.5;

		*centrp = *dt_rise;
		add_to_dt(centrp, 0, *hlenp);
#if 0
		*centrp++ = (trise + tset) * 0.5;
#endif

		SAFE_FREE(dt_rise);
		SAFE_FREE(dt_set);

		centrp++;
		hlenp++;
		lat += dlat_r;

	}

   /* read mask image */

	if (fdm != ERROR) {
		npixels = nlines * nsamps;
		if (pvread (fdm, mbuf, npixels) != npixels) {
			error ("error reading mask image");
		}
	}

   /* loop on kronrod points writing output images of weight and sun angles */

	for (ikt = 0; ikt < kt; ikt++) {

		centrp = centr;
		hlenp = hlen;
		mbufp = mbuf;

		/* get Kronrod weight and abcissa coefficients for this point */

		switch (kt) {
			case 15:
				wg = wgk_15[ikt];
				xg = xgk_15[ikt];
				break;
			case 21:
				wg = wgk_21[ikt];
				xg = xgk_21[ikt];
				break;
		}

		/* open temporary file */

		fdt = uwopen_temp(tname);
		if (fdt == ERROR) {
			error ("Can't open temporary file");
		}

		mins[0] = MAX_WT;
		maxs[0] = MIN_WT;
		mins[1] = MAX_CZ;
		maxs[1] = MIN_CZ;
		mins[2] = MAX_AZM;
		maxs[2] = MIN_AZM;

		lat = DEG_TO_RAD(blat);
		lon = DEG_TO_RAD(blon);

		for (line = 0; line < nlines; line++) {

			/* Kronrod weight for this latitude */

			wt = wg * *hlenp / SECS_IN_DAY;

			mins[0] = MIN (mins[0], wt);
			maxs[0] = MAX (maxs[0], wt);

			/* Kronrod point time for first pixel in line */

#if 0
			time = *centrp + *hlenp * xg;
#endif
			dt = *centrp;
			add_to_dt(&dt, 0, *hlenp * xg);

			obufp = obuf;

			/* loop on samples computing sun angles */

			for (samp = 0; samp < nsamps; samp++) {

		    		if (fdm == ERROR || *mbufp++) {
#if 0
					sunang (lat, lon, time, &cosz, &azm);
#endif
					sunangle(lat, lon, &dt, &cosz, &azm,
						 NULL);

					azm = RAD_TO_DEG(azm);

					mins[1] = MIN (mins[1], cosz);
					maxs[1] = MAX (maxs[1], cosz);
					mins[2] = MIN (mins[2], azm);
					maxs[2] = MAX (maxs[2], azm);

					*obufp++ = wt;
					*obufp++ = cosz;
					*obufp++ = azm;

		   		} else { /* masked point */

					*obufp++ = MIN_WT;
					*obufp++ = MIN_CZ;
					*obufp++ = MIN_AZM;

		   		}

#if 0
				time += delta_t;
#endif
				add_to_dt(&dt, 0, delta_t);
		   		lon += dlon_r;
			}

			/* write line to temporary file */

			if (uwrite (fdt, (addr_t) obuf, onbytes) != onbytes) {
				error ("error writing output buffer, line %d",
					line);
			}

			lat += dlat_r;
			lon = DEG_TO_RAD(blon);
			centrp++;
			hlenp++;

		}

		/* copy temporary file to output image */

		uclose (fdt);
		out_image (tname, ikt, prefix, nlines, nsamps, nbits, mins, maxs);
	}

   /* delete temporary file */

	uremove (tname);
}
