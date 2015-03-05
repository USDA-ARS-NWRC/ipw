/****************************************************************************************
                                FUNCTION EXP_INT3
  Calculates the 3rd exponential integral from "Numerical Recipes in Fortran 90"
  Press, Teukolsky et al; originally coded by M. Tribbeck; recoded in C by D. Marks
  NWRC, Jul 2002
*****************************************************************************************/

#include <math.h>
#include "ipw.h"
#include "pgm.h"

#define THIRD		3
#define MAX_ITER	100
#define EULER		0.5772
#define EPSILON		0.0001
#define NO_CANOPY	(1.0 / (double) (THIRD - 1.0))


double
exp_int3 (
	double	tau	/* optical depth (lai / 2) */)
{
	double	a;
	double	b;
	double	c;
	double	d;
	double	h;
	double	big;
	double	del;
	double	eps;
	double	fact;
	double	psi;
	double	no_canopy;
	double	return_val;

	int	l;
	int	k;

	/*	set no canopy value	*/
	no_canopy = NO_CANOPY;

	/*	set integration limits	*/

	/** don't really understand the f90 functions
	 ** EPSILON(x) and HUGE(x), and can't figure why:
	 ** "eps" is the convergence limit and is set
	 ** to the constant EPSILON (above) but big is confusing...
	 ** if 
	 **	eps = EPSILON(tau)
	 ** then:
	 **	big = HUGE(x) * eps	
	 ** but I'll do it that way...
	 ** I need to figure out a "BIG" value that is reasonable
	 **/

	eps = DBL_EPSILON;
	big = DBL_MAX * eps;

	if (tau < 0.0) {
		/*	bad argument passed	*/
		usrerr("exp_int3: negative canopy optical depth passed");
		return (ERROR);
	}
	else if (tau == 0.0) {
		/*	no canopy, return 0.5	*/
		return (no_canopy);
	}
	else if (tau > 1.0) {
		/*	valid canopy optical depth, greater than 1.0	*/
		/*	use Lentz's algorithm	*/
		b = tau + THIRD;
		c = big;
		d = 1.0 / b;

		h = d;
		k = 1;
		while (k <= MAX_ITER) {
			a = - (double) (k * (THIRD - 1 + k));
			b += 2.0;
			d = 1.0 / ((a * d) + b);
			c = b + (a / c);
			del = c * d;
			h = h * del;
			if (fabs(del - 1.0) <= eps)
				break;
			k++;
		}
		if (k > MAX_ITER)
			error("convergence failed: tau = %g", tau);
		return_val = h * exp(-tau);
		return (return_val);
	}
	else {
		/*	valid optical depth, between 0 and 1	*/
		/*	series representation used to speed convergence	*/
		return_val = no_canopy;
		fact = 1.0;
		k = 1;
		while (k <= MAX_ITER) {
			fact = - fact * tau / (double) k;
			if (k != (THIRD - 1))
				del = - fact / (double) (k - (THIRD - 1));
			else {
				psi = - EULER;
				l = 1;
				while (l <= (THIRD - 1)) {
					psi += 1.0 / (double) l;
					l++;
				}
				del = fact * ( - log(tau) + psi);
			}
			return_val = return_val + del;
			if (fabs(del) < (fabs(return_val) * eps))
				break;
			k++;
		}
		if (k > MAX_ITER)
			error("convergence failed: tau = %g", tau);
		return (return_val);
	}
}
