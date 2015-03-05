/*
** NAME
**	zerobr - finds zero of a function by Brent's algorithm
**
** SYNOPSIS
**	#include "ipw.h"
**
**	double
**	zerobr(
**		double	a,		|* spanning guess for root	*|
**		double	b,		|* spanning guess for root	*|
**		double	t,		|* tolerable relative error	*|
**		double	(*f)() )	|* pointer to function		*|
**
** DESCRIPTION
**	Returns zero of a function by Brent's algorithm (R. Brent,
**	Algorithms for Minimization without Derivatives).
**
** RETURN VALUE
**      errno = 0	zero breakpoint
**
**	errno != 0	error occurred, and message stored via 'usrerr' routine
**			(the function f is also assumed to do this)
**
** HISTORY
**	July 1982:	Written by J. Dozier, Department of Geography, UCSB
**      10/21/92:	Converted to IPW by D. Marks, USGS, EPA, ERL/C;
*/
 
#include <math.h>
#include <errno.h>

#include "ipw.h"
 
double
zerobr(
	double	a,		/* spanning guess for root	*/
	double	b,		/* spanning guess for root	*/
	double	t,		/* tolerable relative error	*/
	double	(*f)() )	/* pointer to function		*/
{
	double	c =0.;
	double	d =0.;
	double	e =0.;
	double	fa;
	double	fb;
	double	fc;
	double	m;
	double	p;
	double	q;
	double	r;
	double	s;
	double	tol;
	double	meps;
	int	maxfun;

	meps = DBL_EPSILON;
	errno = 0;

	/* compute max number of function evaluations */
	if (a == b) {
		usrerr("a = b = %g", a);
		errno = ERROR;
		return(a);
	}

	fb = (ABS(b) >= ABS(a)) ? ABS(b) : ABS(a);
	tol = 5.e-1 * t + 2 * meps * fb;
	s = log(ABS(b - a) / tol) / log(2.);
	maxfun = s * s + 1;

	fa = (*f)(a);
	fc = fb = (*f)(b);
	if (errno) {
		return(0.);
	}

	if (ABS(fb) <= tol)
		return(b);
	if (ABS(fa) <= tol)
		return(a);

	if (fb*fa > 0) {
		usrerr("root not spanned:\n\ta %g, b %g, f(a) %g, f(b) %g",
			a, b, fa, fb);
		errno = ERROR;
		return(0.);
	}

	while (maxfun--) {

		if ((fb > 0 && fc > 0)  ||  (fb <= 0  && fc <= 0))  {
			c = a; 
			fc = fa; 
			d = e = b - a;
		}

		if (ABS(fc) < ABS(fb)) {
			a = b; 
			b = c; 
			c = a;
			fa = fb; 
			fb = fc; 
			fc = fa;
		}

		tol = meps * ABS(b) + t;
		m = (c - b) / 2;

		if (ABS(m) < tol  ||  fb == 0)
			return(b);

		/* see if bisection is forced */
		if (ABS(e) < tol  ||  ABS(fa) <= ABS(fb))
			d = e = m;

		else {
			s = fb/fa;

			if (a == c) {	/* linear interpolation */
				p = 2 * m * s;
				q = 1 - s;
			}

			else {		/* inverse quadratic interpolation */
				q = fa/fc;
				r = fb/fc;
				p = s * (2 * m * q * (q-r) - (b-a) * (r-1));
				q -= 1;
				q *= (r-1) * (s-1);
			}

			if (p > 0)
				q = -q;
			else
				p = -p;

			s = e;
			e = d;

			if (2*p < 3*m*q - ABS(tol*q) && p < ABS(s*q/2))
				d = p/q;
			else
				d = e = m;
		}

		a = b; 
		fa = fb;

		if (ABS(d) > tol)
			b += d;
		else if (m > 0)
			b += tol;
		else
			b -= tol;

		fb = (*f)(b);
		if (errno) {
			return(0.);
		}
	}
	usrerr("did not converge");

	errno = ERROR;
	return(0.);
}
