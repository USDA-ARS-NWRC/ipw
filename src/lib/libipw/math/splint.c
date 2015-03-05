/*
**  NAME
**	splint - integral of spline function
**
**  SYNOPSIS
**	#include "ipw.h"
**
**	double
**	splint(
**		double  *x,	|* vector of abscissas			*|
**		double  *y,	|* vector of ordinates			*|
**		int      nx,	|* length of x & y vectors		*|
**		double  *c,	|* vector of splines of length 3*nx	*|
**		double   a,	|* start of interval of integration	*|
**		double   b)	|* end of interval of integration	*|
**
**  DESCRIPTION
**	returns integral of spline function.  sets 'errno' to ERROR if
**	error occurred, and stores via 'usrerr' routine.
**
**  RETURN VALUE
**	integral of spline function
*/

#include <errno.h>

#include "ipw.h"

double
splint(
	double         *x,	/* vector of abscissas			*/
	double         *y,	/* vector of ordinates			*/
	int             nx,	/* length of x & y vectors		*/
	double         *c,	/* vector of splines of length 3*nx	*/
	double          a,	/* start of interval of integration	*/
	double          b)	/* end of interval of integration	*/
{
	double         *c2;
	double         *c3;
	double         *px;
	double         *py;
	double          d;
	double          da =0.0;
	double          db;
	double          dd =0.0;
	double          dx;
	double          fourth;
	double          half;
	double          q;
	double          qa;
	double          qab;
	double          qb;
	double          sixth;
	double          third;
	double          v;
	int             ipt;
	int             iv =0;
	int             jer;
	int             ker;
	int             nxm2;

	register int    i;
	register int    ia;
	register int    ib;

	if (a == b) {
		q = 0;
		return (q);
	}

	errno = 0;
	sixth = 1.e0 / 6.e0;
	fourth = 2.5e-1;
	third = 1.e0 / 3.e0;
	half = 5.e-1;
	nxm2 = nx - 2;
	jer = ker = ipt = ia = 0;
	v = a < b ? a : b;
	px = x;
	for (;;) {
		d = v - *px++;
		for (i = ia; i <= nxm2; i++) {
			iv = i;
			dd = v - *px++;
			if (dd < 0.)
				break;
			if (i < nxm2)
				d = dd;
		}
		if (dd > 0.)
			ker = -2;
		if (d < 0.)
			jer = -1;
		if (ipt == 2)
			break;
		ipt = 2;
		ia = iv;
		da = d;
		v = a > b ? a : b;
		px = x + ia;
	}
	ib = iv;
	db = d;
	c2 = c + nx;
	c3 = c2 + nx;
	qa = (((*(c3 + ia) * fourth * da + *(c2 + ia) * third)
	       * da + *(c + ia) * half) * da + *(y + ia)) * da;
	qb = (((*(c3 + ib) * fourth * db + *(c2 + ib) * third)
	       * db + *(c + ib) * half) * db + *(y + ib)) * db;
	qab = 0.;
	if (ib > ia) {
		px = x + ia;
		py = y + ia;
		c2 += ia;
		for (i = ia; i < ib; i++) {
			dx = *(px + 1) - *px;
			qab += half * dx *
				(*(py + 1) + (*py) -
				 (*(c2 + 1) + *c2) * dx * dx * sixth);
			px++;
			py++;
			c2++;
		}
	}
	q = a < b ? (qb - qa) + qab : (qa - qb) - qab;

	if (jer + ker != 0) {
		usrerr("a=%g, b=%g, x[0]=%g, x[%d]=%g",
		       a, b, x[0], nx - 1, x[nx - 1]);
		errno = ERROR;
	}

	return (q);
}
