/*
** NAME
** 	pack_dms - convert decimal degrees to packed DMS
** 
** SYNOPSIS
**	#include "mapproj.h"
**
**	double
**	pack_dms(
**		double	ddeg)		|* input value as decimal degrees *|
** 
** DESCRIPTION
**	pack_dms converts the given value in decimal degrees to packed 
**	degrees/minutes/seconds in the format +/-DDDMMSS.SSS where
**	DDD is the degrees, MM is minutes and SS.SSS is seconds.
** 
** RESTRICTIONS
** 
** RETURN VALUE
**	packed DMS value in double precision.
*/

#include <math.h>

#include "ipw.h"
#include "mapproj.h"

double
pack_dms(
	double		ddeg)		/* input value as decimal degrees */
{
	int		sign;		/* sign of input value		 */
	int		deg;		/* degrees part			 */
	int		min;		/* minutes part			 */
	float		sec;		/* seconds part			 */
	double		uvalue;		/* absolute value of given value */
	double		dms;		/* DMS return value		 */


	sign = 1;
	if (ddeg < 0)
		sign = -1;
	uvalue = fabs (ddeg);

	deg = uvalue;
	min = (uvalue - deg) * 60.0;
	sec = (uvalue - deg - min / 60.0) * 3600;

	/* round seconds to nearest 1000th */

	sec = (int) (sec * 1000 + 0.5) / 1000.0;

	if (sec >= 60) {
		sec = 0.0;
		min++;
		if (min >= 60) {
			min = 0;
			deg++;
		}
	}

	dms = ((deg * 10000) + (min * 100) + sec) * sign;

	return (dms);
}
