/*
** NAME
** 	gcp_dist -- return distance and azimuth along great circle path
** 
** SYNOPSIS
**	gcp_dist (sinlat1, coslat1, lon1, sinlat2, coslat2, lon2, distance, azm)
**	double sinlat1, coslat1, lon1;
**	double sinlat2, coslat2, lon2;
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
#include "rearth.h"

void
gcp_dist(
	double		sinlat1,	/* cos start latitude 		 */
	double		coslat1,	/* cos start latitude 		 */
	double		lon1,		/* start longitude (radians)	 */
	double		sinlat2,	/* sin ending latitude 		 */
	double		coslat2,	/* sin ending latitude 		 */
	double		lon2,		/* ending longitude (radians)	 */
	double	       *distance,	/* returned distance (meters)	 */
	double	       *azm)		/* returned azimuth (radians)	 */
{
	double		lon_diff;	/* lon2 - lon1			 */
	double		cos_lon_diff;	/* cos(lon_diff)		 */
	double		sin_lon_diff;	/* sin(lon_diff)		 */
	double		sin_c;		/* sin(gcp distance)		 */
	double		cos_c;		/* cos(gcp distance)		 */
	double		tan_azm;	/* tan(azm)			 */
	double		factor;

	lon_diff = lon2 - lon1;
	sin_lon_diff = sin (lon_diff);
	cos_lon_diff = cos (lon_diff);

	cos_c = sinlat1 * sinlat2 + coslat1 * coslat2 * cos_lon_diff;

	factor = coslat1 * sinlat2 - sinlat1 * coslat2 * cos_lon_diff;
	sin_c = sqrt (coslat2 * coslat2 * sin_lon_diff * sin_lon_diff +
		      factor * factor);

	*distance = atan2 (sin_c, cos_c) * RAD_EARTH;

	tan_azm = coslat2 * sin_lon_diff / factor;

	*azm = atan (tan_azm);
}

#ifdef TEST_MAIN

main(void)
{
	double lat1, lon1;
	double lat2, lon2;
	double rlat1, rlon1;
	double rlat2, rlon2;
	double dist, azm;
	double sinlat1, coslat1;
	double sinlat2, coslat2;

	printf ("Enter latitude and longitude of start point: ");
	scanf ("%lf %lf", &lat1, &lon1);

	printf ("Enter latitude and longitude of end point: ");
	scanf ("%lf %lf", &lat2, &lon2);

	rlat1 = lat1 * PIO180;
	rlat2 = lat2 * PIO180;
	rlon1 = lon1 * PIO180;
	rlon2 = lon2 * PIO180;

	sinlat1 = sin (rlat1);
	sinlat2 = sin (rlat2);
	coslat1 = cos (rlat1);
	coslat2 = cos (rlat2);

	gcp_dist (sinlat1, coslat1, rlon1, sinlat2, coslat2, rlon2, &dist, &azm);
	printf ("distance %f, azimuth %f\n", dist, azm);
}
#endif
