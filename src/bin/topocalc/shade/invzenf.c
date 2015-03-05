/*
 * convert cosine to zenith in degrees and radians
 * check for error
 */

#include <math.h>

#include "ipw.h"

void
invzenf(
	double          u0,		/* cos (zenith)	 */
	double         *zenith,		/* zenith in radians	 */
	double         *zend)		/* zenith in degrees	 */
{
	if (u0 <= 0 || u0 > 1) {
		error("-u %g: must be > 0 and <= 1", u0);
	}
	*zend = *zenith = acos(u0);
	*zend *= 90 / acos(0.);
}
