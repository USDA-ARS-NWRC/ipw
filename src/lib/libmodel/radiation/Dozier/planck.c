/*
**  name
**	Planck functions for blackbody radiation
**
**  synopsis
**	double	planck(lambda, temp)
**	double	lambda, temp;
**
**	double	plkfq(freq, temp)
**	double	freq, temp;
**
**	double	plkwn(wavenum, temp)
**	double	wavenum, temp;
**
**	double	tbwlen(lambda, rad)
**	double	lambda, rad;
**
**	double	tbfq(freq, rad)
**	double	freq, rad;
**
**	double	tbwn(wavenum, rad)
**	double	wavenum, rad;
**
**  description
**	Planck returns blackbody exitance in W m^-2 um^-1 sr^-1
**	Plkfq returns blackbody exitance in W m^-2 sec sr^-1
**	Plkwn returns blackbody exitance in W m^-2 cm sr^-1
**	Tbwlen, tbfq, and tbwn return brightness temperature in deg K,
**	for the specified wavelength, frequency, or wavenumber and
**	the corresponding radiant exitance.
**
**	lambda	wavelength in micrometers
**	temp	temperature in degrees K
**
**	freq	frequency in sec^-1
**
**	wavenum	wavenumber in cm^-1
**
**  diagnostics
**	Dies with message on error.
**
**  history
**	January 1983: written by J. Dozier, Department of Geography, UCSB
**
*/

#include <math.h>
#include "ipw.h"

/* Define these again just in case someone didn't I suppose...*/

extern double   EXFUN(exp, (double x));
extern double   EXFUN(frexp, (double value, int *exp));
extern double   EXFUN(ldexp, (double x, int exp));
extern double   EXFUN(log, (double x));
extern double   EXFUN(plkgen, (double x, int n, int *expo));
extern double   EXFUN(ploggen, (double y, int n, int *expo));

extern int      errno;

 /*
  * some Planck equation constants expressed as mantissa & exponent, to
  * prevent underflow or overflow in intermediate calculations
  * 
  * h	Planck's constant
  * c	speed of light
  * k	Boltzmann's constant
  */

 /*
  * 2 h c^2 = twohc2 * 2^thn
  */
static double   twohc2 = 0.53615617035935;
static int      thn = -52;

 /*
  * hc / k = hcf * 2^hcn
  */
static double   hcf = 0.920608233608797;
static int      hcn = -6;

 /*
  * h/k = hkf * 2^hkn
  */
static double   hkf = 0.824509089626284;
static int      hkn = -34;

 /*
  * h = hf * 2^hn
  */
static double   hf = 0.860126820722472;
static int      hn = -110;

 /*
  * c = cf * 2^cn
  */
static double   cf = 0.558276582509279;
static int      cn = 29;

double
planck(lambda, temp)
	register double lambda;
	register double temp;
{
	double          lf;
	double          rad;
	double          x;
	double          y;
	int             n;
	int             nl;

	if (lambda == 0. || temp == 0.)
		return (0.);

	if (lambda < 0.) {
		error("planck: lambda = %g", lambda);
	}
	if (temp < 0.) {
		error("planck: temperature = %g", temp);
	}

 /* convert lambda to meters */
	lambda *= 1.e-6;

 /*
  * normalized variable, expressed as mantissa and exponent i.e.
  * x = (h c) / (k lambda T) , y = 1/(e^x-1)
  */
	x = hcf * frexp(1 / (lambda * temp), &n);
	if ((y = plkgen(x, n + hcn, &n)) == 0)
		return ((double) 0);
 /*
  * radiant exitance in SI units, expressed as mantissa & exponent i.e.
  * rad = 2 y h c^2 / lambda^5
  */
	lf = frexp(1 / lambda, &nl);
	rad = y * twohc2 * lf * lf * lf * lf * lf;
	n += thn + 5 * nl;

 /* convert from SI to conventional units  */
	return (1.e-6 * ldexp(rad, n));
}

double
plkfq(freq, temp)
	register double freq;
	register double temp;
{
	double          ff;
	double          rad;
	double          x;
	double          y;
	int             fn;
	int             n;

	if (freq == 0 || temp == 0) {
		return ((double) 0);
	}

	if (freq < 0) {
		error("plkfq: frequency = %g", freq);
	}
	if (temp < 0) {
		error("plkfq: temperature = %g", temp);
	}

 /*
  * normalized variable, expressed as mantissa and exponent i.e.
  * x = h nu / k T, y = 1/(e^x-1)
  */

	x = hkf * frexp((freq / temp), &n);
	if ((y = plkgen(x, hkn + n, &n)) == 0) {
		return ((double) 0);
	}

 /* radiant exitance, 2 h nu^3 y / c^2  */
	ff = frexp(freq, &fn);
	rad = hf * ff * y;
	ff /= cf;
	rad *= ff * ff;
	n += hn + fn * 3 + 1 - cn * 2;

	return ldexp(rad, n);
}

double
plkwn(wavenum, temp)
	register double wavenum;
	register double temp;
{
	double          rad;
	double          wf;
	double          x;
	double          y;
	int             n;
	int             wn;

	if (wavenum == 0 || temp == 0) {
		return ((double) 0);
	}

	if (wavenum < 0) {
		error("plkwn: wavenumber = %g", wavenum);
	}
	if (temp < 0) {
		error("plkwn: temperature = %g", temp);
	}

 /* convert wavenum to m^-1  */
	wavenum *= 100;

 /*
  * normalized variable, separated into mantissa & exponent
  * x = h c eta / k T, y = 1/(e^x-1)
  */
	x = hcf * frexp(wavenum / temp, &n);
	if ((y = plkgen(x, n + hcn, &n)) == 0)
		return ((double) 0);

 /* radiant exitance in SI units  */
	wf = frexp(wavenum, &wn);
	rad = twohc2 * y * wf * wf * wf;
	n += wn * 3 + thn;

 /* convert to conventional units  */
	return (1.e2 * ldexp(rad, n));
}

 /*
  * returns 1/(exp(x)-1)
  */
double
plkgen(x, n, expo)
	double          x;
	int             n;
	int            *expo;
{
	register double y;

	errno = 0;
	x = ldexp(x, n);
	y = exp(x);

 /*
  * error from exp(), we check y also because of documentation bug in
  * UNIX exp() write-up
  */

	if (!errno && y > 1) {
		--y;
	}
	else {
 /* reset error flag  */
		errno = 0;

 /* underflow, use Rayleigh-Jeans approximation  */
		if (y <= 1)
			y = x;

 /* overflow  */
		else {
			*expo = 0;
			return ((double) 0);
		}
	}

	y = frexp(1 / y, expo);

	return y;
}

double
tbwlen(lambda, rad)
	register double lambda;
	register double rad;
{
	double          lf;
	int             ln;
	double          rf;
	int             rn;
	double          y;
	double          z;
	int             zn;

	if (lambda <= 0) {
		if (lambda < 0) {
			error("tbwlen: lambda = %g", lambda);
		}
		return ((double) 0);
	}
	if (rad <= 0) {
		if (rad < 0) {
			error("tbwlen: rad = %g", rad);
		}
		return ((double) 0);
	}

 /* convert wavelength to (SI units) meters  */
	lf = frexp(1.e-6 * lambda, &ln);

 /* convert radiance to (SI units) W m^-3 sr^-1  */
	rf = frexp(1.e6 * rad, &rn);

 /*
  * normalized variables
  * y = 2 h c^2 / rad lambda^5
  * z = 1/log(1+y)
  */

	y = twohc2 / (rf * lf * lf * lf * lf * lf);
	z = ploggen(y, thn - rn - 5 * ln, &zn);

 /* h c / k lambda log(1+y) */
	z *= hcf / lf;
	zn += hcn - ln;

	return ldexp(z, zn);
}

double
tbfq(freq, rad)
	register double freq;
	register double rad;
{
	double          ff;
	int             fn;
	double          rf;
	int             rn;
	double          y;
	int             yyn;

	if (freq <= 0) {
		if (freq < 0) {
			error("tbfq: frequency = %g", freq);
		}
		return ((double) 0);
	}
	if (rad <= 0) {
		if (rad < 0) {
			error("tbfq: rad = %g", rad);
		}
		return ((double) 0);
	}

 /* mantissas and exponents */
	ff = frexp(freq, &fn);
	rf = frexp(rad, &rn);

 /*
  * normalized variable y = 2 h nu^3 / rad c^2
  */

	y = hf * ff * ff * ff / (rf * cf * cf);
	yyn = 1 + hn + 3 * fn - rn - 2 * cn;

	y = ploggen(y, yyn, &yyn);
	y *= hkf * ff;
	yyn += hkn + fn;

	return ldexp(y, yyn);
}

double
tbwn(wavenum, rad)
	register double wavenum;
	register double rad;
{
	double          rf;
	int             rn;
	double          wf;
	int             wn;
	double          y;
	int             yyn;

	if (wavenum <= 0) {
		if (wavenum < 0) {
			error("tbwn: wavenumber = %g", wavenum);
		}
		return (0.);
	}
	if (rad <= 0.) {
		if (rad < 0.) {
			error("rad = %g", rad);
		}
		return (0.);
	}

 /* convert wavenumber to (SI units) m^-1  */
	wf = frexp(wavenum * 1.e2, &wn);

 /* convert radiance to (SI units) W m^-1 sr^-1  */
	rf = frexp(rad * 1.e-2, &rn);

 /*
  * normalized variable 
  * y = 2 h c^2 eta^3 / rad
  */
	y = ploggen(twohc2 * wf * wf * wf / rf, thn + 3 * wn - rn, &yyn);

 /* h c eta / k log(1+y) */
	y *= hcf * wf;
	yyn += hcn + wn;

	return (ldexp(y, yyn));
}

 /*
  * returns 1/log(1+y), as mantissa and exponent
  */
double
ploggen(y, n, expo)
	double          y;
	int             n;
	int            *expo;
{
	double          z;

	z = frexp(1. / log(1. + ldexp(y, n)), expo);

	return (z);
}

#ifndef	lint
ID_STR(RCS_id,"$Header: planck.c,v 1.2 88/03/28 13:05:40 dozier Exp $")

#endif
