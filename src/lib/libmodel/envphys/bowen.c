#include	"ipw.h"
#include	"envphys.h"

double
bowen(
	double	p,	/* air pressure (same units as ea and es)  */
	double	ta,	/* potential air temperature (K)	   */
	double  ts,	/* potential surface temperature (K)	   */
	double  ea,	/* vapor pressure (same units as p and es) */
	double	es)	/* vapor pressure at surface (same units
			     as p and es)			   */
{
	double	cb, tmean;

	tmean = (ta + ts)/2.;
	cb = p * CP_AIR / ((MOL_H2O/MOL_AIR)*LH_VAP(tmean));

	if (ea == es)
		error("ea = es = %lg",ea);

	return(cb*(ta-ts)/(ea-es));
}
