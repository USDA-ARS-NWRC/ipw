#include "ipw.h"
#include "radiation.h"

void
delta_edd(
	double         *omega,	/* single-scattering albedo	  */
	double         *g,	/* scattering asymmetry parameter */
	double         *tau)	/* optical depth		  */
{
	double          gp1;		/* g + 1			 */
	double          gstar;		/* g sup star			 */
	double          omg;
	double          ostar;		/* omega sup star		 */

	assert(*omega >= 0 && *omega <= 1);
	assert(*g >= 0 && *g <= 1);
	assert(*tau > 0);

	gp1 = 1 + *g;
	gstar = *g / gp1;
	omg = 1 - *g * *g * *omega;
	ostar = ((1 - *g) * gp1 * *omega) / omg;

	*tau *= omg;
	*omega = ostar;
	*g = gstar;
}
