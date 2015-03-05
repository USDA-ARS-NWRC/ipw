/* Given lat/lon in decimal degrees for two points, return great-circle
 * distance in radians.
 */
#include <stdio.h>
#include <math.h>

#define EARTH_RADIUS 3958.89    /* mean radius in statute miles */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG  57.2957795130823208768

double geodist(
	double		lat1,
	double		lon1,
	double		lat2,
	double		lon2)
{
    double dlon, cosD, D;

    if ((lat1 == lat2) && (lon1 == lon2)) {
        return(0.0);
    }

    /* convert to radians */
    lat1 /= DEG;
    lon1 /= DEG;
    lat2 /= DEG;
    lon2 /= DEG;

    dlon = lon2 - lon1;
    cosD = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(dlon);
    D = acos(cosD);

    return(D);

}

