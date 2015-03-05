/* Given longitudes in radians and sin/cos of latitudes, return great-circle
 * distance in radians.
 */
#include <stdio.h>
#include <math.h>

double geodist(
	double	coslat1,
	double	sinlat1,
	double	coslat2,
	double	sinlat2,
	double	lonrad1,
	double	lonrad2)
{
    double dlon, cosD, D;

    dlon = lonrad2 - lonrad1;
    cosD = sinlat1 * sinlat2 + coslat1 * coslat2 * cos(dlon);
    D = acos(cosD);

/*** fprintf(stderr,"D = %g \n", D);
***/

    return(D);

}

