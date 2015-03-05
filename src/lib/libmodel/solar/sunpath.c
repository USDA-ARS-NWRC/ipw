#include <math.h>

#include "ipw.h"
#include "solar.h"

int
sunpath(
	double          lat,		/* latitude (radians)		 */
	double          lon,		/* longitude (radians)		 */
	double          declin,		/* solar declination (radians)	 */
	double          omega,		/* solar longitude (radians)	 */
	double         *cosZ,		/* cosine of solar zenith	 */
	double         *azm)		/* solar azimuth (radins)	 */
{
 /*
  * Check arguments: latitude must be between +/- 90 degrees, longitude
  * between +/- 180 degrees, declination between +/- MAX_DECLIN
  * (defined above), and solar longitude between +/- 180 degrees. If
  * outside range, write appropriate error message via usrerr() and
  * return (ERROR).
  */
	if (fabs(lat) > M_PI_2) {
		usrerr("latitude (%g deg) not between -90 and +90 degrees",
		       RAD_TO_DEG(lat));
		return (ERROR);
	}
	if (fabs(lon) > M_PI) {
		usrerr("longitude (%g deg) not between -180 and +180 degrees",
		       RAD_TO_DEG(lon));
		return (ERROR);
	}
	if (fabs(declin) > DEG_TO_RAD(MAX_DECLINATION)) {
		usrerr("declination (%g deg) > maximum declination (%g deg)",
		       RAD_TO_DEG(declin), MAX_DECLINATION);
		return (ERROR);
	}
	if (fabs(omega) > M_PI) {
	   usrerr("longitude of sun (%g deg) not between -180 and +180 degrees",
		  RAD_TO_DEG(omega));
	   return (ERROR);
	}
 /*
  * Use general axis rotation routine to compute sun angle.
  */
	if (rotate(sin(declin), omega, sin(lat), lon, cosZ, azm) == ERROR) {
		return (ERROR);
	}
 /*
  * set flag if sun is below horizon
  */
	if (*cosZ < 0.) {
		return (1);
	}
 /*
  * normal return
  */
	return (0);
}
