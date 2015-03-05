/*
** NAME
** 	gsunlight -- calculate sun angles for global-scale lat/lon image
** 
** SYNOPSIS
**	void
**	gsunlight (fdm, fdo, year, month, day, hour, min, sec,
**		   blat, blon, dlat, dlon)
**	int fdm, fdo;
**	int year, month, day, hour, min, sec;
**	double blat, blon, dlat, dlon;
** 
** DESCRIPTION
** 	Gsunlight calculates sun angles (cos zenith angle and azimuth)
**	for each pixel in the output image for the given date and time.
** 
*/

#include <time.h>
#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"
#include "solar.h"

void
gsunlight (
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output image file desc	 */
	int		year,		/* year for calculations 	 */
	int		month,		/* month for calculations (1-12) */
	int		day,		/* day for calculations (1-31)	 */
	int		hour,		/* hour for calculations (0-23)	 */
	int		min,		/* minute for calculations (0-59)*/
	int		sec,		/* second for calculations (0-59)*/
	double		blat,		/* UL latitude in degrees	 */
	double		blon,		/* UL longitude in degrees	 */
	double		dlat,		/* latitude spacing in degrees	 */
	double		dlon)		/* longitude spacing in degrees	 */
{
	int		nsamps;		/* # samps in output image	 */
	int		nlines;		/* # lines in output image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* samp loop counter		 */
	double          rv;		/* radius vector (not used)	 */
	double          declin;		/* declination			 */
	double          omega;		/* solar longitude		 */
	double          lat;		/* latitude (radians)	 	 */
	double          lon;		/* longitude (radians)	 	 */
	double		blat_r;		/* UL latitude (radians)	 */
	double		blon_r;		/* UL longitude (radians)	 */
	double		dlat_r;		/* latitude spacing (radians)	 */
	double		dlon_r;		/* longitude spacing (radians)	 */
	double		azm;		/* solar azimuth (radians)	 */
	double		mu;		/* cos solar zenith angle	 */
	pixel_t        *mbuf;		/* -> mask line buffer		 */
	pixel_t        *mbufp;		/* -> in mask line buffer	 */
	fpixel_t       *obuf;		/* -> output line buffer	 */
	fpixel_t       *obufp;		/* -> in output line buffer	 */
	datetime_t     *dt;		/* date-time structure		 */


	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);

   /* allocate mask line buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("error allocating mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* allocate output line buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("error allocating output buffer");
	}

   /* get sun position at given time */

	dt = make_dt(year, month, day, hour, min, sec);
	if (dt == NULL)
		error("invalid date-time");
	if (ephemeris (dt, &rv, &declin, &omega) == ERROR)
		error ("ephemeris");

   /* convert latitudes and longitudes to radians */

	blat_r = DEG_TO_RAD(blat);
	blon_r = DEG_TO_RAD(blon);
	dlat_r = DEG_TO_RAD(dlat);
	dlon_r = DEG_TO_RAD(dlon);

	lat = blat_r;
	lon = blon_r;

   /* loop on image lines */

	for (line = 0; line < nlines; line++) {

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image - line %d", line);
			}
		}

		obufp = obuf;
		mbufp = mbuf;

		/* loop on samples */

		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp) {

				/* compute sun angles */

				switch (sunpath (lat, lon, declin, omega,
						&mu, &azm)) {

					case 1:		/* sun below horizon */
						mu = 0;
						break;
					case ERROR:
						error ("sunpath");
					}

				/* assign output buffer */

				*obufp++ = mu;
				*obufp++ = RAD_TO_DEG(azm);

		   	} else { /* masked point */

				*obufp++ = 0.0;
				*obufp++ = 0.0;
		   	}
	
		   	mbufp++;
			lon += dlon_r;
		}

		/* write output line */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("error writing output file, line %d", line);
		}

		lat += dlat_r;
		lon = blon_r;
	}
}
