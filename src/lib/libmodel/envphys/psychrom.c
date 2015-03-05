#include <math.h>

#include "ipw.h"
#include "envphys.h"

double
psychrom(
	double	tdry,	/* dry bulb temperature (K) */
	double	twet,	/* wet bulb temperature (K) */
	double	press)	/* total air pressure (Pa)  */
{
	double	qa;
	double	qw;
	double	xlh;
	double	xlhv;
	double	xlhf;
	double	fu_fac;
	double	e;

	if (tdry <= 0. || twet <= 0. || press <= 0. || tdry < twet)
		error("tdry, twet, press negative or zero");

	/* find latent heat of vaporization, or vaporization + fusion */
	if (tdry <= FREEZE) {
		xlhv = LH_VAP((tdry + twet) / 2.0);
		xlhf = LH_FUS((tdry + twet) / 2.0);
		xlh  = xlhv + xlhf;
	}
	else if (twet <= FREEZE) {
		xlhv = LH_VAP((tdry + twet) / 2.0);
		xlhf = LH_FUS((FREEZE + twet) / 2.0);
		fu_fac = ((FREEZE - twet) / (tdry - twet));
		xlh  = xlhv + (fu_fac * xlhf);
	}
	else
		xlh = LH_VAP((tdry+twet)/2);

	/* saturation specific humidity at twet */
	e = sati(twet);
	qw = SPEC_HUM(e,press);

	/* specific humidity */
	qa = qw - (CP_AIR/xlh) * (tdry-twet);

	/* check result */
	if (qa < 0.0)
		error("combination of tdry, twet & press not possible");

	/* convert to vapor pressure */
	e = INV_SPEC_HUM(qa,press);

	return(e);
}
