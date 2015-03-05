/*
**	solint - integral of solar constant from wavelengths a to b
**		(in micrometers)
**	solwnt - integral of solar constant from wavenumbers a to b
**		(in cm^-1)
**
*/

#include <math.h>
#include <errno.h>

#include "ipw.h"
#include "pgm.h"
#include "solar.h"

double
solint(
	double   a,
	double   b)
{
	static int      already = 0;
	static double   c[3 * NSOL];

	double         *wave;
	double         *val;
	double          intgrl;

	wave = &solar_data[0][0];
	val = &solar_data[1][0];

 /*
  * first pass: initialize
  */
	if (!already) {
		already = 1;

 /*
  * calculate splines
  */
		if (akcoef(wave, val, NSOL, c) == ERROR)
			error("solint: akcoef error");
	}

	errno = 0;
	intgrl = fabs(splint(wave, val, NSOL, c, a, b));
	if (errno)
		error("solint: splint error");

	return (intgrl * SOLAR_CONSTANT);
}

double
solwnt(
	int             a,
	int             b)
{
	static int      already = 0;
	static double   c[3 * NSOL];
	static double   wn[NSOL];
	static double   wnval[NSOL];
	double          intgrl;
	int             j;

 /*
  * first pass: initialize
  */
	if (!already) {
		already = 1;

 /*
  * convert wavelengths to wavenumbers
  */
		for (j = 0; j < NSOL; j++)
			wn[j] = WAVENO(solar_data[0][j]);

 /*
  * sort into increasing order
  */
		SORT_ALG(wn, NSOL, sizeof(double), updbl);

 /*
  * solar const in W m^-2 cm
  */
		for (j = 0; j < NSOL; j++)
			wnval[j] = solwn((int) wn[j]);

 /*
  * calculate splines
  */
		if (akcoef(wn, wnval, NSOL, c) == ERROR)
			error("solwnt: akcoef error");
	}

	errno = 0;
	intgrl = fabs(splint(wn, wnval, NSOL, c, (double) a, (double) b));
	if (errno)
		error("solwnt: splint error");

	return (intgrl);
}
