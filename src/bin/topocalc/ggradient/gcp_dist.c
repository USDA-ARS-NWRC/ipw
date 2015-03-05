/*
** NAME
** 	gcp_dist -- return distance and azimuth along great circle path
** 
** SYNOPSIS
**	gcp_dist (lat1, lon1, lat2, lon2, distance, azm)
**	double lat1, lon1;
**	double lat2, lon2;
**	double *distance;
**	double *azm;
** 
** DESCRIPTION
** 	gcp_dist returns the distance in meters and azimuth in radians (East of
**	North) between the two given latitude, longitude pairs.  The longitudes
**	are assumed to be in radians (-pi to pi) and the latitudes are in
**	radians from -pi (S. Pole) to pi (N. Pole).
** 
*/

#include <math.h>
#include "ipw.h"
#include "pgm.h"

#define RAD_EARTH	(double) 6370949.0	/* radius of earth in m	 */

void
gcp_dist(
	double		lat1,		/* start latitude (radians)	 */
	double		lon1,		/* start longitude (radians)	 */
	double		lat2,		/* ending latitude (radians)	 */
	double		lon2,		/* ending longitude (radians)	 */
	double	       *distance,	/* returned distance (meters)	 */
	double	       *azm)		/* returned azimuth (radians)	 */
{
	double		cos_lat1;	/* cos(lat1)			 */
	double		sin_lat1;	/* sin(lat1)			 */
	double		cos_lat2;	/* cos(lat2)			 */
	double		sin_lat2;	/* sin(lat2)			 */
	double		lon_diff;	/* lon2 - lon1			 */
	double		cos_lon_diff;	/* cos(lon_diff)		 */
	double		sin_lon_diff;	/* sin(lon_diff)		 */
	double		sin_c;		/* sin(gcp distance)		 */
	double		cos_c;		/* cos(gcp distance)		 */
	double		tan_azm;	/* tan(azm)			 */
	double		factor;

	sin_lat1 = sin (lat1);
	sin_lat2 = sin (lat2);
	cos_lat1 = cos (lat1);
	cos_lat2 = cos (lat2);

	lon_diff = lon2 - lon1;
	sin_lon_diff = sin (lon_diff);
	cos_lon_diff = cos (lon_diff);

	cos_c = sin_lat1 * sin_lat2 + cos_lat1 * cos_lat2 * cos_lon_diff;

	factor = cos_lat1 * sin_lat2 - sin_lat1 * cos_lat2 * cos_lon_diff;
	sin_c = sqrt (cos_lat2 * cos_lat2 * sin_lon_diff * sin_lon_diff +
		      factor * factor);

	*distance = atan2 (sin_c, cos_c) * RAD_EARTH;

	tan_azm = cos_lat2 * sin_lon_diff / factor;

	*azm = atan (tan_azm);
	if (*azm > M_PI)
		*azm -= 2 * M_PI;
	else if (*azm < -M_PI)
		*azm += 2 * M_PI;
}
