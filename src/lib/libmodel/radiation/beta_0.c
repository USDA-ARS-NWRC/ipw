#include <math.h>

#include "ipw.h"
#include "radiation.h"

#define TOL	FLT_EPSILON
#define MAXNO	2048

double
beta_0(
	double	u0,	/* cosine of illumination angle   */
	double	g)	/* scattering asymmetry parameter */
{
	int             n;		/* index	 		 */
	double          m;		/* half index			 */
	double          fm;		/* coefficient	 		 */
	double          gn;		/* coefficient			 */
	double          sum;		/* sum of Legendre series	 */
	double          last =0.0;	/* previous sum	 		 */
	double          pn;		/* Legendre polynomial P sub n	 */
	double          pnm1;		/* P sub (n-1)			 */
	double          pnm2;		/* P sub (n-2)			 */

 /*
  * we find the integral-sum
  * 
  * sum (n=0 to inf) g^n * (2*n+1) * Pn(u0) * int (u'=0 to 1) Pn(u')
  * 
  * note that int of Pn vanishes for even values of n (Abramowitz &
  * Stegun, eq 22.13.8-9); therefore the series becomes
  * 
  * sum (n=0 to inf) g^n * (2*n+1) * Pn(u0) * f(m)
  * 
  * where 2*m+1 = n and the f's are computed recursively
  */
 /*
  * check input quantities
  */
	assert(u0 > 0 && u0 <= 1);
	assert(g >= 0 && g <= 1);

 /*
  * Legendre polynomials of degree 0 and 1
  */
	pnm2 = 1;
	pnm1 = u0;
 /*
  * first coefficients and initial sum
  */
	fm = ((double) (-1)) / ((double) 8);
	gn = 7 * g * g * g;
	sum = 3 * g * u0 / 2;
 /*
  * sum until convergence; we use the even terms only for the recursive
  * calculation of the Legendre polynomials
  */
	if (g != 0 && u0 != 0) {
		for (n = 2; n < MAXNO; ++n) {
 /*
  * order n Legendre polynomial
  */
			pn = ((2 * n - 1) * u0 * pnm1 + (1 - n) * pnm2) / n;
 /*
  * even terms vanish
  */
			if (ODD(n)) {
				last = sum;
				sum = last + gn * fm * pn;
				if (fabs((sum - last) / sum) < TOL && sum <= 1)
					break;
 /*
  * recursively find next f(m) and gn coefficients
  * 
  * n = 2 * m + 1
  */
				m = (n - 1) / 2;
				fm *= -(2 * m + 1) / (2 * (m + 2));
				gn *= g * g * (4 * m + 7) / (4 * m + 3);
			}
 /*
  * ready to compute next Legendre polynomial
  */
			pnm2 = pnm1;
			pnm1 = pn;
		}
 /*
  * warn if no convergence
  */
		if (n == MAXNO) {
			warn("%s: %s - u0=%g g=%g sum=%g last=%g fm=%g",
			     "betanaught", "no convergence",
			     u0, g, sum, last, fm);
			if (sum > 1)
				sum = 1;
		}
		else {
			assert(sum >= 0 && sum <= 1);
		}
	}
	else {
		sum = 0;
	}

 /*
  * beta_0
  */
	return ((1 - sum) / 2);
}
