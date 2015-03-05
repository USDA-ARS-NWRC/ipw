/*
**  NAME
**	solval - spectral value of solar constant, W ^ m^-2 um^-1
**	solwn - spectral value of solar constant, W ^ m^-2 cm
**
**  SYNOPSIS
**	double solval(lambda)
**	double lambda;
**	double solwn(waveno)
**	double waveno;
**
**  DESCRIPTION
**	Solval returns spectral value of solar constant at wavelength
**	lambda (in micrometers).  Solwn returns spectral value at
**	wavenumber waveno (in cm^-1).
**
**  DIAGNOSTICS
**
**  BUGS
*/

#include "ipw.h"
#include "pgm.h"
#include "solar.h"

double
solval(
	double          lambda)
{
	static int      already = 0;
	static double   c[3 * NSOL];

	double         *wave;
	double         *val;
	double          value;


	wave = &solar_data[0][0];
	val = &solar_data[1][0];

 /*
  * first pass: initialization
  */
	if (!already) {
		already = 1;

 /*
  * calculate splines
  */
		if (akcoef(wave, val, NSOL, c) == ERROR)
			error("solval: akcoef error");
	}

	if (lambda < wave[0] || lambda > wave[NSOL - 1])
		return (0.);

	value = spval1(lambda, wave, val, NSOL, c);

	return (value * SOLAR_CONSTANT);
}

double
solwn(
	int             eta)
{
	double          lambda;
	double          result;

 /*
  * convert inverse cm to wavelength in um
  */
	lambda = WAVELEN(eta);

	result = solval(lambda);
 /*
  * convert W m^-2 um^-1 to W m^-2 cm
  */
	lambda *= 1.e-6;		/* -> meters */
	result *= 1.e-6;		/* -> per meter */
	result *= lambda * lambda;	/* -> W m^-2 m */
	result *= 1.e2;			/* -> W m^-2 cm */

	return (result);
}
