/* akcoef - spline coefficients for Akima interpolation */

#include "ipw.h"
#include "pgm.h"

/*----------------------------------------------------------------------------
 * akcoef - computes coefficients for Akima-type spline interpolation
 *----------------------------------------------------------------------------
 */

int
akcoef(
	double         *x,
	double         *y,
	int             nx,
	double         *c)
{
	double          t1;
	double          t2;
	double          b;
	double          rm1;
	double          rm2;
	double          rm3;
	double          rm4;
	double         *holdx;
	double         *holdy;
	double         *c1;
	double         *c2;
	double         *c3;

	register int    i;
	register int    n0;

	if (nx <= 3) {
		usrerr("akcoef: # pts <= 3");
		return (ERROR);
	}

	holdx = x;

	for (i = 1; i < nx; i++) {
		if (x[1] <= x[0]) {
			usrerr("akcoef: input vector not increasing order");
			return (ERROR);
		}
		x++;
	}

	x = holdx;
	rm3 = (y[1] - y[0]) / (x[1] - x[0]);
	t1 = rm3 - (y[1] - y[2]) / (x[1] - x[2]);
	rm2 = rm3 + t1;
	rm1 = rm2 + t1;
	n0 = nx - 2;
	holdy = y;
	c1 = c;
	c2 = c + nx;
	c3 = c2 + nx;

	for (i = 0; i < nx; i++) {
		if (i >= n0)
			rm4 = rm3 - rm2 + rm3;
		else {
			x++;
			y++;
			rm4 = (y[1] - *y) / (x[1] - *x);
		}

		t1 = rm4 - rm3;
		t2 = rm2 - rm1;
		if (t1 < 0)
			t1 = -t1;
		if (t2 < 0)
			t2 = -t2;
		b = t1 + t2;

		*c1++ = (b == 0.) ? 5.e-1 * (rm2 + rm3) : (t1 * rm2 + t2 * rm3) / b;
		rm1 = rm2;
		rm2 = rm3;
		rm3 = rm4;
	}

	x = holdx;
	y = holdy;
	c1 = c;
	n0 = nx - 1;

	for (i = 0; i < n0; i++) {
		t1 = 1.e0 / (x[1] - *x);
		t2 = t1 * (y[1] - *y);
		b = (*c1 + *(c1 + 1) - t2 - t2) * t1;
		b = (*c1 + c1[1] - t2 - t2) * t1;
		*c3++ = t1 * b;
		*c2++ = -b + (t2 - *(c1++)) * t1;
		x++;
		y++;
	}

	return (OK);
}
