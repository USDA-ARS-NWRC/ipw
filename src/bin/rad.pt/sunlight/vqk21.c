/*
**	21-pt Kronrod quadrature pts and wts on [a,b], normalized to
**	produce average over tlen
*/

#include "ipw.h"
#include "pgm.h"

#define NKPT 21
#define NGPT 10

 /*
  * abscissae and weights copied from quadpack
  */

static double   xgk[] = {
	0.9956571630258081e+00, 0.9739065285171717e+00,
	0.9301574913557082e+00, 0.8650633666889845e+00,
	0.7808177265864169e+00, 0.6794095682990244e+00,
	0.5627571346686047e+00, 0.4333953941292472e+00,
	0.2943928627014602e+00, 0.1488743389816312e+00,
	0.0000000000000000e+00
};

static double   wgk[] = {
	0.1169463886737187e-01, 0.3255816230796473e-01,
	0.5475589657435200e-01, 0.7503967481091995e-01,
	0.9312545458369761e-01, 0.1093871588022976e+00,
	0.1234919762620659e+00, 0.1347092173114733e+00,
	0.1427759385770601e+00, 0.1477391049013385e+00,
	0.1494455540029169e+00
};

void
vqk21(
	double          tlen,
	double          a,
	double          b,
	double         *x,
	double         *w)
{
	vqkagn(NKPT, NGPT, tlen, a, b, x, w, xgk, wgk);
}
