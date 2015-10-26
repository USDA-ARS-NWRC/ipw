/*
 * 2015-10-26 Scott Havens
 * Modify dew_point.c to accept a tolerance argument
 */

#include "ipw.h"
#include "envphys.h"

/* ------------------------------------------------------------------------ */

static double e_pass;
#pragma omp threadprivate(e_pass)

static double
satm(double t)
{
	return(e_pass - sati(t));
}

/* ------------------------------------------------------------------------ */

double
dew_pointp(
		double e, 	/* vapor pressure (Pa) */
		double tol)	/* tolerance threshold */
{
	double	a;
	double	b;
	double	result;

	if (e < 0 || e > 1.5*SEA_LEVEL) {
		error("dew_point: vapor pressure = %g", e);
	}

	/* select starting guesses to span root */

	/* lower */
	a = FREEZE;
	while (e < sati(a))
		a *= .75;

	/* upper */
	b = FREEZE + 15;
	while (e > sati(b))
		b *= 1.25;

	e_pass = e;
	result = zerobr(a, b, tol, satm);

	return(result);
}
