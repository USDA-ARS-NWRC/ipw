#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"

double
ri_no(
	double	z2,	/* upper height (m) */
	double	z1,	/* lower height (m) (maybe roughness length) */
	double	t2,	/* virtual temperature at z2 (K) */
	double	t1,	/*    "        "       "  z1  "  */
	double	u2,	/* wind speed at z2 (m/s) */
	double	u1)	/*  "     "   "  z1   "   (may be zero) */
{
	double	tgm;
	double	du;

	if (z2 <= z1 || z2 <= 0. || z2 <= 0.)
		error("z2=%g <= z1", z2);
	if (u2 <= u1 || u2 <= 0. || u1 < 0.)
		error("u2=%g <= u1", u2);

	if (u2 == 0.)
		return(0.);

	/* geometric mean virtual temperature */
	tgm = sqrt(t2*t1);

	/* wind speed difference */
	du = u2 - u1;

	return(GRAVITY*(z2-z1)*(t2-t1)/(tgm*du*du));
}
