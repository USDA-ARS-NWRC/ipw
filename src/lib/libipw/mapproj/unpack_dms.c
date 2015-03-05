/*
** NAME
** 	unpack_dms - convert packed DMS to decimal degrees
** 
** SYNOPSIS
**	#include "mapproj.h"
**
**	double
**	unpack_dms(
**		double	dms)		|* input value in packed DMS	 *|
** 
** DESCRIPTION
**	unpack_dms converts the given value in packed DMS format
**	(degrees/minutes/seconds in the format +/-DDDMMSS.SSS where
**	DDD is the degrees, MM is minutes and SS.SSS is seconds.) to
**	decimal degrees.
** 
** RETURN VALUE
**	DMS value in double precision.
*/

#include <math.h>

#include "ipw.h"
#include "mapproj.h"

double
unpack_dms(
	double		dms)		/* input value in packed DMS	 */
{
	int		sign;		/* sign of input value		 */
	int		deg;		/* degrees part			 */
	int		min;		/* minutes part			 */
	float		sec;		/* seconds part			 */
	double		uvalue;		/* absolute value of given value */
	double		ddeg;		/* decimal degrees return value	 */


	if (dms >= 0) {
		sign = 1;
		uvalue = dms;
	} else {
		sign = -1;
		uvalue = -dms;
	}

	deg = uvalue / 10000.0;
	min = fmod (uvalue, 10000.0) / 100.0;
	sec = fmod (uvalue, 100.0);

	ddeg = sign * (deg + min / 60.0 + sec / 3600.0);

	return (ddeg);
}
