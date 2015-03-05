/*
**  NAME
**	rotate - rotation of spherical coordinates
**
**  SYNOPSIS
**	#include "ipw.h"
**
**	int
**	rotate(
**		double    mu,		|* cosine of angle theta from z-axis *|
**		double    azm,		|* azimuth (+ccw from x-axis) 	     *|
**		double    mu_r,		|* cosine of angle theta_r of
**					   rotation of z-axis		     *|
**		double    lam_r,	|* azimuth (+ccw) of x-axis rotation *|
**		double   *muPrime,	|* new cosine of angle from z'-axis  *|
**		double   *aPrime)	|* new azimuth from x'-axis	     *|
**
**  DESCRIPTION
**	Calculates new spherical coordinates if system rotated about
**	origin.  Coordinates are right-hand system.  All angles are in
**	radians.
**
**	input --
**	mu	cosine of angle theta from z-axis in old coordinate system
**	azm	azimuth (+ccw from x-axis) in old coordinate system
**	mu_r	cosine of angle theta_r of rotation of z-axis
**	lam_r	azimuth (+ccw) of rotation of x-axis
**
**	output --
**	muPrime	new value of cosine of angle from z'-axis
**	aPrime	new value of azimuth from x'-axis
**
**	for example, to use this program to calculate sun angles, set:
**
**	mu	= sin(declination)
**	azm	= hour angle of sun (long. where sun is vertical)
**	mu_r	= sin(latitude)
**	lam_r	= longitude
**
**	output will be muPrime = cosZ and aPrime = solar azimuth
**
**  RETURN VALUE
**	OK	calculations succeeded, and output parameters assigned.
**
**	ERROR	error in arguments, and message stored via 'usrerr' routine.
*/

#include <math.h>
#include <errno.h>

#include "ipw.h"

int
rotate(
	double          mu,		/* cosine of angle theta from z-axis */
	double          azm,		/* azimuth (+ccw from x-axis) 	     */
	double          mu_r,		/* cosine of angle theta_r of
					   rotation of z-axis		     */
	double          lam_r,		/* azimuth (+ccw) of x-axis rotation */
	double         *muPrime,	/* new cosine of angle from z'-axis  */
	double         *aPrime)		/* new azimuth from x'-axis	     */
{
 /*
  * Note: theta = arc cos(mu)
  */
	double          cosOmega;	/* cos(omega)		 */
	double          omega;		/* lam_r - azm		 */
	double          sinTheta;	/* sin(theta)		 */
	double          sinThr;		/* sin(theta-sub-r)	 */

 /*
  * Check input values: mu = cos(theta); mu_r = cos(theta-sub-r)
  */
	if (mu < -1. || mu > 1.) {
		usrerr("rotate: mu = cos(theta) = %g is not between -1 and +1",
			mu);
		return (ERROR);
	}
	if (mu_r < -1. || mu_r > 1.) {
		usrerr("rotate: mu_rotate = cos(theta-sub-r) = %g is not between -1 and +1", mu_r);
		return (ERROR);
	}
	if (fabs(azm) > 2 * M_PI) {
		usrerr("rotate: azimuth (%g deg) is not between -360 and 360",
			RAD_TO_DEG(azm * 360));
		return (ERROR);
	}
	if (fabs(lam_r) > 2 * M_PI) {
		usrerr("rotate: lam_r (% deg) = axis rotation, is not between -360 and 360",
		       RAD_TO_DEG(lam_r));
		return (ERROR);
	}

 /*
  * difference between azimuth and rotation angle of x-axis
  */
	omega = lam_r - azm;
 /*
  * set Unix error indicator before calling trig routines; check after
  * calling for trig error
  */
	errno = 0;
 /*
  * sine of angle theta (cosine is an argument);
  * sine of angle theta-sub-r (cosine is an argument);
  * bug if sqrt triggers error
  */
	sinTheta = sqrt((1. - mu) * (1. + mu));
	sinThr = sqrt((1. - mu_r) * (1. + mu_r));
	assert(errno == 0);
 /*
  * cosine of difference between azimuth and aziumth of rotation
  */
	cosOmega = cos(omega);
 /*
  * Output:
  * azimuth and cosine of angle in rotated axis system.
  * (bug if trig routines trigger error)
  */
	*aPrime = -atan2(sinTheta * sin(omega),
			 mu_r * sinTheta * cosOmega - mu * sinThr);
	*muPrime = sinTheta * sinThr * cosOmega + mu * mu_r;
	assert(errno == 0);

	return (OK);
}
