/*
**      Generalized function to assign pts and weights for Kronrod
**	quadrature
**	nkpt - # Kronrod pts
**	ngpt - # Gauss pts
*/

#include "ipw.h"
#include "pgm.h"

void
vqkagn(
	int		nkpt,
	int		ngpt,
	double          tlen,
	double          a,
	double          b,
	double         *x,
	double         *w,
	double         *xgk,
	double         *wgk)
{
	double          centr;
	double          hlgth;
	double          absc;
	int             j;
	int             jtw;
	int             jtwm1;
	double         *xg;
	double         *wg;

	centr = (a + b) / 2;
	hlgth = (b - a) / 2;

 /*
  * copied from Fortran, use Fortran indexing
  */

	xg = &xgk[-1];
	wg = &wgk[-1];

	*x++ = centr;
	*w++ = wg[nkpt / 2 + 1] * hlgth / tlen;

	for (j = ngpt / 2; j >= 1; --j) {
		jtw = j * 2;
		absc = hlgth * xg[jtw];
		*x++ = centr - absc;
		*w++ = wg[jtw] * hlgth / tlen;
		*x++ = centr + absc;
		*w++ = wg[jtw] * hlgth / tlen;
	}

	for (j = nkpt / 2 - ngpt / 2; j >= 1; --j) {
		jtwm1 = j * 2 - 1;
		absc = hlgth * xg[jtwm1];
		*x++ = centr - absc;
		*w++ = wg[jtwm1] * hlgth / tlen;
		*x++ = centr + absc;
		*w++ = wg[jtwm1] * hlgth / tlen;
	}
}
