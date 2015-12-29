/*
 * convert zenith in degrees to radians
 * check for error
 */

#include <math.h>

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"

double
zenf(
	double		zend)		/* zenith in degrees	*/
{
	double		zenith;		/* zenith in radians	*/
	double		pid2;		/* pi / 2		*/

	if (zend < 0 || zend >= 90) {
		error("-z %g: must be >= 0 and < 90", zend);
	}
	pid2 = acos(0.);
	zenith = zend * pid2 / 90;

	return (zenith);
}
