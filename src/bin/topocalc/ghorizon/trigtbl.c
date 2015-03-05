/*
** NAME
** 	trigtbl -- initialize trigonmetric lookup tables for ghorizon
** 
** SYNOPSIS
**	void
**	trigtbl (fds, got_solar, maskout, asintbl, acostbl, tzentbl, zenith,
**		 sinlon, coslon, blon, dlon, nsamps,
**		 sindist, cosdist, max_pts, step_size)
** 	int fds;
**	bool_t got_solar;
**	bool_t maskout;
**	float *acostbl, *asintbl, *tzentbl;
**	bool_t zenith;
**	double *coslon, *sinlon;
**	double blon, dlon;
**	int nsamps;
**	double *sindist, *cosdist;
**	int maxpts;
**	double step_size;
** 
** DESCRIPTION
** 	trigtbl initializes lookup tables for sines and cosines of all
**	possible solar azimuth angles in the input images to program ghorizon,
**	as well as the sines and cosines of all image grid latitudes, longitudes,
**	and distance values.  A floating point conversion map is also initialized
**	for solar zenith angles in radians.
**	Note: sine and cosine tables of solar azimuth switch the direction of
**	the azimuth so that +PI is the N.Pole instead of the S.Pole.
** 
*/

#include <math.h>
#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
trigtbl(
        int             fds,		/* solar file descriptor	  */
	bool_t		got_solar,	/* flag if solar image included	  */
	bool_t		maskout,	/* flag for mask image output	  */
	float          *asintbl,	/* all possible values of sin azm */
	float          *acostbl,	/* all possible values of cos azm */
	float          *tzentbl,	/* map for tangent zenith	  */
	bool_t		zenith,		/* flag set if zenith in image    */
	double	       *sinlon,		/* all values of sin lon	  */
	double	       *coslon,		/* all values of cos lon	  */
	double		blon,		/* start longitude (radians)	  */
	double		dlon,		/* longitude spacing (radians)	  */
	int		nsamps,		/* number of image samples	  */
	double	       *sindist,	/* all values of sin(distance)	  */
	double	       *cosdist,	/* all values of cos(distance)	  */
	int		maxpts,		/* number of distance points	  */
	double		step_size)	/* distance step size (radians)	  */
{
	REG_1 int	j;		/* loop counter and B.F.	 */
	fpixel_t      **map;		/* -> F.P. maps			 */
	fpixel_t       *amap;		/* -> F.P. map of azimuth band	 */
	fpixel_t       *zmap;		/* -> F.P. map of zenith band	 */
	int            *maplen;		/* -> lengths of F.P. maps	 */
	int		max;		/* length of F.P. map (max B.F.) */
	float	       *pcos;		/* -> in cos array		 */
	float	       *psin;		/* -> in sin array		 */
	float	       *ztbl;		/* -> in tzentbl array		 */
	double	       *dcos;		/* -> in coslon array		 */
	double	       *dsin;		/* -> in sinlon array		 */
	double		azm;		/* azimuth in degrees		 */
	double		azmr;		/* azimuth in radians		 */
	double		zr;		/* zenith in radians		 */
	double		lon;		/* longitude in radians		 */
	double		rdist;		/* distance in radians		 */

   /* sines and cosines of each possible azimuth angle */
   /* add 180 degrees to azimuth to change from +S/-N to -S/+N */

	if (got_solar) {
		map = fpmap (fds);
		maplen = fpmaplen (fds);
		if (maskout) {
			amap = map[1];
			max = maplen[1];
		} else {
			amap = map[0];
			max = maplen[0];
		}

		pcos = acostbl;
		psin = asintbl;

		for (j = 0; j < max; j++) {
			azm = 180.0 - *amap;
			if (azm > 180.0)
				azm -= 360.0;
			azmr = azm * PIO180;
			*pcos++ = cos (azmr);
			*psin++ = sin (azmr);
			amap++;
		}
	} else {
		map = NULL;
		maplen = NULL;
	}

   /* if zenith is set, band 0 is zenith in degrees; else band 0 is cos zenith */

	if (maskout) {
		zmap = map[0];
		ztbl = tzentbl;
		max = maplen[0];

		if (zenith) {
			for (j = 0; j < max; j++) {
				zr = *zmap * PIO180;
				*ztbl++ = tan (M_PI_2 - zr);
				zmap++;
			}
		} else {
			for (j = 0; j < max; j++) {
				zr = acos ((double) *zmap);
				*ztbl++ = tan (M_PI_2 - zr);
				zmap++;
			}
		}
	}

   /* compute sines and cosines of each image grid longitude */

	lon = blon;
	dcos = coslon;
	dsin = sinlon;

	for (j = 0; j < nsamps; j++) {
		*dcos++ = cos (lon);
		*dsin++ = sin (lon);
		lon += dlon;
	}

   /* compute sines and cosines of each distance for GCP calculation */

	rdist = step_size;
	dcos = cosdist;
	dsin = sindist;

	for (j = 0; j < maxpts; j++) {
		*dcos++ = cos (rdist);
		*dsin++ = sin (rdist);
		rdist += step_size;
	}

}
