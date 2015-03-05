/*
**	15-pt Kronrod quadrature pts and wts on [a,b], normalized to
**	produce average over tlen
*/

#include "ipw.h"
#include "pgm.h"

#define NKPT 15
#define NGPT 7

 /*
  * weights and abscissae copied from quadpack
  */

static double   xgk[] = {
	0.9914553711208126e+00, 0.9491079123427585e+00,
	0.8648644233597691e+00, 0.7415311855993944e+00,
	0.5860872354676911e+00, 0.4058451513773972e+00,
	0.2077849550078985e+00, 0.0e+00
};

static double   wgk[] = {
	0.2293532201052922e-01, 0.6309209262997855e-01,
	0.1047900103222502e+00, 0.1406532597155259e+00,
	0.1690047266392679e+00, 0.1903505780647854e+00,
	0.2044329400752989e+00, 0.2094821410847278e+00
};

void
vqk15(
	double          tlen,
	double          a,
	double          b,
	double         *x,
	double         *w)
{
	vqkagn(NKPT, NGPT, tlen, a, b, x, w, xgk, wgk);
}
