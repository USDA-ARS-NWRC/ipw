/*
 * convert azimuth in degrees to radians
 * check for error, correct for possible rounding error
 */

#include <math.h>

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"

double
azmf(
	double		azd)		/* azimuth in degrees	*/
{
	double		azimuth;	/* azimuth in radians	*/

	if (fabs(azd) > 180) {
		error("-a %g: must be between -180 and +180", azd);
	}
	azimuth = azd * M_PI / 180;

	/* correct for slight rounding error */
	if (azimuth > M_PI) {
		azimuth = M_PI;
	}
	else if (azimuth < -M_PI) {
		azimuth = -M_PI;
	}

	return (azimuth);
}
