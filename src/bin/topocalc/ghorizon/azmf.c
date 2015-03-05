/*
** NAME
** 	azmf -- convert azimuth in degrees to radians
** 
** SYNOPSIS
**	double azmf (azd)
**	double azd;
** 
** DESCRIPTION
**	convert azimuth in degrees to radians
**	check for error, correct for possible rounding error
** 
*/

#include <math.h>

#include "ipw.h"
#include "pgm.h"

double
azmf(
	double		azd)		/* azimuth in degrees	*/
{
	double		azimuth;	/* azimuth in radians	*/

	if (fabs(azd) > 180) {
		error("-a %g: must be between -180 and +180", azd);
	}
	azimuth = azd * PIO180;

	/* correct for slight rounding error */
	if (azimuth > M_PI) {
		azimuth = M_PI;
	}
	else if (azimuth < -M_PI) {
		azimuth = -M_PI;
	}

	return (azimuth);
}
