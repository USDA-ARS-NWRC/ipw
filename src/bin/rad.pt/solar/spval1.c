/* spval1 - single spline interpolation value */

#include <math.h>
#include "ipw.h"
#include "pgm.h"

double
spval1(
	double   u,
	double   x[],
	double   y[],
	int      n,
	double   c[])
{
	char		str[80];
	register	i;
	register	j;
	register	k;
	register double	*cur;

	i = 0;
	j = n - 1;

	/* check range */
	if (u < x[i]  ||  u > x[j]) {
		/* re-adjust if difference is small */
		if (u < x[i] && fabs(x[i]-u) < 2*u*DBL_EPSILON)
			u = x[i];
		else if (u > x[j] && fabs(u-x[j]) < 2*u*DBL_EPSILON)
			u = x[j];
		else {
			sprintf(str, "u %g  x[0] %g  x[%d] %g",
			u, x[0], j, x[j]);
			bug(str);
		}	
	}

	/* find spanning values using binary search */
	while (i <= j) {
		k = (i+j) / 2;
		cur = &x[k];

		if (u > *cur)
			i = k + 1;

		else if (u < *cur)
			j = k - 1;

		else		/* exact case, u == x[k] */
			return (y[k]);
	}

	u -= x[j];
	return (((c[2*n+j] * u + c[n+j]) * u + c[j]) * u + y[j]);
}
