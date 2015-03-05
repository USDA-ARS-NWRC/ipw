/*
** NAME
** 	gcp -- calculate latitude and longitude of point along GCP
** 
** SYNOPSIS
**	gcp (sin_dist, cos_dist, lat, lon)
**	double sin_dist, cos_dist;
**	double *lat, *lon;
** 
** DESCRIPTION
** 	gcp returns the latitude and longitude of the point the given
**	distance from the start point and azimuth initialized with init_gcp.
**	The returned latitude is in radians (-PI South Pole to PI North Pole);
**	the longitude is in radians (-PI to PI) from 0 E.
**
**	Formulas for great circle path calculations were derived from:
**	Snyder, John P., Map Projections Used by the U.S. Geological Survey,
**	1982, Geological Survey Bulletin 1532, p. 33-36.
** 
*/

#include <math.h>
#include "ipw.h"
#include "pgm.h"
#include "rearth.h"

static double start_lon;	/* lon of start point (radians)		 */
static double cos_slat;		/* cos of latitude of start point	 */
static double sin_slat;		/* sin of latitude of start point	 */
static double cos_slon;		/* cos of lonigtude of start point	 */
static double sin_slon;		/* sin of longitude of start point	 */
static double cos_azm;		/* cos of azimuth			 */
static double sin_azm;		/* sin of azimuth			 */
static double cos_factor;	/* cos_slat * cos_azm			 */

void
gcp(
        double		sin_dist,	/* sin(distance along GCP)	 */
	double		cos_dist,	/* cos(distance along GCP)	 */
	double		*lat,		/* return latitude (radians)	 */
	double		*lon)		/* return longitude (radians)	 */
{
	double		a_lat;	/* latitude in radians from N. Pole	 */
	double		C_lon;	/* longitude difference in radians	 */


   /* compute latitude as arc distance from North Pole to new point */
   /* return latitude is from -PI (S.Pole) to PI (N.Pole) */

	a_lat = acos (cos_dist * sin_slat + sin_dist * cos_factor);
	*lat = M_PI_2 - a_lat;

   /* compute angular distance between longitudes - add to longitude of start */

	C_lon = asin (sin_azm * sin_dist / sin(a_lat));
	*lon = start_lon + C_lon;

	if (*lon < -M_PI)
		*lon += TWOPI;
	else if (*lon > M_PI)
		*lon -= TWOPI;
}

/*
** NAME
** 	init_gcp -- initialize great circle path calculations
** 
** SYNOPSIS
**	init_gcp (start_lon, sinlat, coslat, sinlon, coslon, sinazm, cosazm)
**	double start_lon;
**	double sinlat, coslat;
**	double sinlon, coslon;
**	double sinazm, cosazm;
** 
** DESCRIPTION
** 	init_gcp initializes the calculations of latitude and longitude from
**	a start lat/lon location, distance and azimuth.  init_gcp is called
**	once with the cossines and sines of the start latitude (radians
**	-PI South Pole to PI North Pole) and longitude (radians from 0 E),
**	and the sin and cos of the azimuth (radians from 0N).  Then
**	gcp_to_latlon can be called successively to return latitude and
**	longitude given a distance from the start point.
** 
** RESTRICTIONS
**	start longitude must be between PI and -PI
** 
*/

void
init_gcp(
	double		slon,		/* start longitude (radians)	 */
	double		sinlat,		/* sin of start latitude	 */
	double		coslat,		/* cos of start latitude	 */
	double		sinlon,		/* sin of start longitude	 */
	double		coslon,		/* cos of start longitude	 */
	double		sinazm,		/* sin of azimuth of gcp	 */
	double		cosazm)		/* cos of azimuth of gcp	 */
{

   /* initialize globals for gcp calls */

	start_lon = slon;
	if (start_lon < -M_PI || start_lon > M_PI)
		error ("init_gcp: longitude value not in range (-PI,PI)");

	cos_slat = coslat;
	sin_slat = sinlat;
	cos_slon = coslon;
	sin_slon = sinlon;
	cos_azm = cosazm;
	sin_azm = sinazm;
	cos_factor = cos_slat * cos_azm;
}

#ifdef TEST_MAIN

main (void)
{
	double lat1, lon1;
	double lat2, lon2;
	double azm;
	double rlat1, rlat2, rlon1, rlon2, razm;
	double distance;
	double sinlat, coslat;
	double sinlon, coslon;
	double sinazm, cosazm;
	double deg2rad;
	double pi;
	double two_pi;
	double pi_2;
	double dist_r;

	pi = atan2 (0.0, -1.0);
	two_pi = pi * 2.0;
	pi_2 = pi / 2.0;
	deg2rad = pi / 180.0;

	printf ("input lat1 lon1 in decimal degrees: ");
	scanf ("%lf %lf", &lat1, &lon1);

	printf ("azimuth in decimal degrees: ");
	scanf ("%lf", &azm);

	printf ("distance in meters: ");
	scanf ("%lf", &distance);

	rlat1 = lat1 * deg2rad;
	rlon1 = lon1 * deg2rad;
	razm = azm * deg2rad;
	dist_r = distance / RAD_EARTH;

	sinlat = sin (rlat1);
	coslat = cos (rlat1);
	sinlon = sin (rlon1);
	coslon = cos (rlon1);
	sinazm = sin (razm);
	cosazm = cos (razm);

	init_gcp (rlon1, sinlat, coslat, sinlon, coslon, sinazm, cosazm);

	gcp (sin(dist_r), cos(dist_r), &rlat2, &rlon2);
	printf ("lat2, lon2 %f %f\n", rlat2 / deg2rad, rlon2 / deg2rad);
}

void
error (char *string)
{
	fprintf (stderr, "ERROR: %s\n", string);
	exit (1);
}
#endif
