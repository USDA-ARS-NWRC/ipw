/*
**  NAME
**	ephemeris - calculates ephemeris data
**
**  SYNOPSIS
**	#include "solar.h"
**
**	int
**	ephemeris(
**		datetime_t  *dt,	|* date-time (GMT)		 *|
**		double      *r,		|* radius vector		 *|
**		double      *declin,	|* declination (radians, +north) *|
**		double      *omega)	|* sun longitude (radians +east) *|
**
**  DESCRIPTION
**	Calculates radius vector, declination, and apparent longitude
**	of sun, as function of the given date and time.
**
**	The routine is adapted from:
**
**	W. H. Wilson, Solar ephemeris algorithm, Reference 80-13, 70
**	     pp., Scripps Institution of Oceanography, University of
**	     California, San Diego, La Jolla, CA, 1980.
**
**  RETURN VALUE
**	OK	calculations succeeded, and output parameters assigned.
**
**	ERROR   error occurred, and message stored via 'usrerr' routine.
*/

#include <math.h>
#include <errno.h>

#include "ipw.h"
#include "solar.h"

#define	JULIAN_CENTURY	36525		/* days in Julian century */
#define	TOLERANCE	FLT_EPSILON

/* ------------------------------------------------------------------------ */

 /*
  * modified from /usr/src/lib/libF77/d_sign.c
  */

static double
dsign(
	double          a,
	double          b)
{
	double          x;

	x = (a >= 0 ? a : -a);
	return (b >= 0 ? x : -x);
}

/* ------------------------------------------------------------------------ */

 /*
  * original ibm 360/44 fortran ivf - vislab - wilson - 29jul79
  * translated, modified and reduced by dozier - ucsb - 11/81
  */

int
ephemeris(
	datetime_t     *dt,		/* date-time (GMT)		 */
	double         *r,		/* radius vector		 */
	double         *declin,		/* declination (radians, +north) */
	double         *omega)		/* sun longitude (radians +east) */
{
	double          degrd;
	double          one;
	double		gmts;		/* seconds since midnight GMT */
	double          p11;
	double          p12;
	double          p13;
	double          p14;
	double          p15;
	double          p22;
	double          p23;
	double          p24;
	double          p25;
	double          p26;
	double          p27;
	double          p28;
	double          p29;
	double          p30;
	double          p31;
	double          p32;
	double          p33;
	double          p34;
	double          p35;
	double          p36;
	double          p41;
	double          p43;
	double          p44;
	double          p45;
	double          p46;
	double          p47;
	double          p48;
	double          p51;

 /*
  * Clear the error flag
  */
	errno = 0;

	one = 1;
	degrd = atan(one) / 45;

 /*
  * Convert time to seconds since midnight
  */
	gmts = HR_TO_SEC(dt->hour) + MIN_TO_SEC(dt->min) + dt->sec;

 /*
  * p51 = gmts/3600/24*360
  */
	p51 = gmts / 10.0 / 24.0;
	p22 = ((dt->year - 1900) * JULIAN_CENTURY - 25) / 100
	      + yearday(dt->year, dt->month, dt->day) - 0.5;
	p23 = (p51 / DEGS_IN_CIRCLE + p22) / JULIAN_CENTURY;
	p22 = p23 * JULIAN_CENTURY;
 /*
  * mean longitude - p24
  */
	p11 = 279.69668;
	p12 = 0.9856473354;
	p13 = 3.03e-4;
	p24 = p11 + fmod(p12 * p22, DEGS_IN_CIRCLE) + p13 * p23 * p23;
	p24 = fmod(p24, DEGS_IN_CIRCLE);
 /*
  * mean anomaly - p25
  */
	p11 = 358.47583;
	p12 = 0.985600267;
	p13 = -1.5e-4;
	p14 = -3.e-6;
	p25 = p11 + fmod(p12 * p22, DEGS_IN_CIRCLE)
		  + p23 * p23 * (p13 + p14 * p23);
	p25 = fmod(p25, DEGS_IN_CIRCLE);
 /*
  * eccentricity - p26
  */
	p11 = 0.01675104;
	p12 = -4.18e-5;
	p13 = -1.26e-7;
	p26 = p11 + p23 * (p12 + p13 * p23);
	p11 = p25 * degrd;
	p12 = p11;

	do {
		p13 = p12;
		p12 = p11 + p26 * sin(p13);
	}
	while (fabs((p12 - p13) / p12) > TOLERANCE);

	p13 = p12 / degrd;
 /*
  * true anomaly - p27
  */
	p27 = 2.0 * atan(sqrt((1.0 + p26) / (1.0 - p26))
			 * tan(p13 / 2.0 * degrd)) / degrd;
	if (dsign(1.0, p27) != dsign(1.0, sin(p13 * degrd)))
		p27 += 1.8e2;
	if (p27 < 0.0)
		p27 += DEGS_IN_CIRCLE;
 /*
  * radius vector - r
  */
	*r = 1.0 - p26 * cos(p13 * degrd);
 /*
  * aberration - p29
  */
	p29 = SEC_TO_HR(-20.47 / (*r));
 /*
  * mean obliquity - p43
  */
	p11 = 23.452294;
	p12 = -0.0130125;
	p13 = -1.64e-6;
	p14 = 5.03e-7;
	p43 = p11 + p23 * (p12 + p23 * (p13 + p14 * p23));
 /*
  * mean ascension - p45
  */
	p11 = 279.6909832;
	p12 = 0.98564734;
	p13 = 3.8707;
	p13 = 3.8708e-4;
	p45 = p11 + fmod(p12 * p22, DEGS_IN_CIRCLE) + p13 * p23 * p23;
	p45 = fmod(p45, DEGS_IN_CIRCLE);
 /*
  * nutation and longitude pert
  */
	p11 = 296.104608;
	p12 = 1325 * DEGS_IN_CIRCLE;
	p13 = 198.8491083;
	p14 = 0.00919167;
	p15 = 1.4388e-5;
	p28 = p11 + fmod(p12 * p23, DEGS_IN_CIRCLE)
		+ p23 * (p13 + p23 * (p14 + p15 * p23));
	p28 = fmod(p28, DEGS_IN_CIRCLE);
 /*
  * mean elongation of moon - p30
  */
	p11 = 350.737486;
	p12 = 1236 * DEGS_IN_CIRCLE;
	p13 = 307.1142167;
	p14 = 1.436e-3;
	p30 = p11 + fmod(p12 * p23, DEGS_IN_CIRCLE) + p23 * (p13 + p14 * p23);
	p30 = fmod(p30, DEGS_IN_CIRCLE);
 /*
  * moon long of ascending node - p31
  */
	p11 = 259.183275;
	p12 = -5 * DEGS_IN_CIRCLE;
	p13 = -134.142008;
	p14 = 2.0778e-3;
	p31 = p11 + fmod(p12 * p23, DEGS_IN_CIRCLE) + p23 * (p13 + p14 * p23);
	p31 = fmod(p31, DEGS_IN_CIRCLE);
 /*
  * mean long of moon - p31
  */
	p11 = 270.434164;
	p12 = 1336 * DEGS_IN_CIRCLE;
	p13 = 307.8831417;
	p14 = -1.1333e-3;
	p32 = p11 + fmod(p12 * p23, DEGS_IN_CIRCLE) + p23 * (p13 + p14 * p23);
	p32 = fmod(p32, DEGS_IN_CIRCLE);
 /*
  * moon perturbation of sun long - p33
  */
	p33 = 6.454 * sin(p30 * degrd) + 0.013 * sin(3 * p30 * degrd) +
		0.177 * sin((p30 + p28) * degrd) -
		0.424 * sin((p30 - p28) * degrd);
	p33 = SEC_TO_HR(p33);
 /*
  * nutation of long - p34
  */
	p34 = -(17.234 - 0.017 * p23) * sin(p31 * degrd) +
		0.209 * sin(2 * p31 * degrd) -
		0.204 * sin(2 * p32 * degrd);
	p34 = p34 - 1.257 * sin(2 * p24 * degrd) + 0.127 * sin(p28 * degrd);
	p34 = SEC_TO_HR(p34);
 /*
  * nutation in obliquity - p34
  */
	p35 = 9.214 * cos(p31 * degrd) + 0.546 * cos(2 * p24 * degrd) -
		.09 * cos(2 * p31 * degrd) + 0.088 * cos(2 * p32 * degrd);
	p35 = SEC_TO_HR(p35);
 /*
  * inequalities of long period - p36
  */
	p36 = 0.266 * sin((31.8 + 119 * p23) * degrd) +
		((1.882 - 0.016 * p23) * degrd) *
		sin((57.24 + 150.27 * p23) * degrd);
	p36 = p36 + 0.202 * sin((315.6 + 893.3 * p23) * degrd) +
		1.089 * p23 * p23 + 6.4 * sin((231.19 + 20.2 * p23) * degrd);
	p36 = SEC_TO_HR(p36);
 /*
  * apparent longitude - p41
  */
	p41 = p27 - p25 + p24 + p29 + p33 + p36 + p34;

	p43 += p35;
 /*
  * apparent right ascension - p44
  */
	p44 = atan(tan(p41 * degrd) * cos(p43 * degrd)) / degrd;
	if (dsign(one, p44) != dsign(one, sin(p41 * degrd)))
		p44 += 1.8e2;
	if (p44 < 0.)
		p44 += DEGS_IN_CIRCLE;
 /*
  * equation of time - p46
  */
	p46 = p45 - p44;
	if (p46 > 1.8e2)
		p46 -= DEGS_IN_CIRCLE;
 /*
  * declination - p47
  */
	p47 = asin(sin(p41 * degrd) * sin(p43 * degrd));
	*declin = p47;
 /*
  * hour angle in degrees - p48
  */
	p48 = p51 + p46 - 1.8e2;
	if (p48 > 180)
		p48 -= DEGS_IN_CIRCLE;
	else if (p48 < -180)
		p48 += DEGS_IN_CIRCLE;
	*omega = -p48 * degrd;

	if (errno) {
		syserr();
		return (ERROR);
	}
	else
		return (OK);
}
