#include <math.h>

#include "ipw.h"
#include "envphys.h"

#define RH2O 461.5		/* Gas constant for water vapor (J/kg/K) */
#define EPS (MOL_H2O/MOL_AIR)	/* Ratio of moleculr weights of water and dry air */
#define CONVERGE 0.0001		/* Convergence value */

double
wetbulb(
	double	ta,	/* air tempterature (K) */
	double	dpt,	/* dewpoint temperature (K) */
	double	press)	/* total air pressure (Pa)  */
{
	int	i;
	double	ea;	/* vapor pressure (Pa) */
	double	esat;	/* saturation ea @ Ta (Pa) */
	double	xlh;	/* latent heat of vaporization + fusion (sublimation) (J/kg) */
	double	xlhv;	/* latent heat of vaporization *(J/kg) */
	double	xlhf;	/* latent heat of fusion *(J/kg) */
	double	fu_fac;	/* fudge factor for xlh stradeling 0 */
	double	psyc;	/* Psychrometric "constant" (K/Pa) */
	double	dedt;	/* Change in ea with temperature (Pa/K) */
	double	pf;	/* Psychrometer value (K) */
	double	dpdt;	/* Change in pf with temperature */
	double	ti;	/* wet or ice bulb temperature (K) */
	double	ti0;	/* initial value for ti */
	double	dti;	/* closure value */

	/* find latent heat of vaporization, or vaporization + fusion */
	if (ta <= FREEZE) {
		xlhv = LH_VAP((ta + dpt) / 2.0);
		xlhf = LH_FUS((ta + dpt) / 2.0);
		xlh  = xlhv + xlhf;
	}
	else if (dpt <= FREEZE) {
		xlhv = LH_VAP((ta + dpt) / 2.0);
		xlhf = LH_FUS((FREEZE + dpt) / 2.0);
		fu_fac = ((FREEZE - dpt) / (ta - dpt));
		xlh  = xlhv + (fu_fac * xlhf);
	}
	else
		xlh = LH_VAP((ta+dpt)/2);

	/* vapor pressure and saturation vapor pressure at ta */
	ea = sati(dpt);
	esat = sati(ta);

	/* Psychrometric "constant" (K/Pa) */
	psyc = EPS * (xlh / (CP_AIR * press));

	/* solve for wet or ice bulb temperature */
	dti = 1.0;
	i = 0;
	ti = ta; 
	while (dti > CONVERGE) {
		ti0 = ti;
		if (ti != ta)
			esat = sati(ti);
		dedt = xlh * (esat / (RH2O * (ti*ti)));
		pf = (ti - ta) + (psyc * (esat - ea));
		dpdt = 1.0 + (psyc * dedt);
		ti = ti - (pf / dpdt);
		dti = ti0 - ti;
		i++;
		if (i > 10)
			error("failure to converge in 10 iterations");
	}
	return(ti);
}
