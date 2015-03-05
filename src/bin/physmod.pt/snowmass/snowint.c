/* 
	function snowint   calculate snow depth after interception sublimation 
	removed.

*/

#include  <math.h>

#include "ipw.h"
#include "envphys.h"
#include "radiation.h"
#include "snow.h"
#include "pgm.h"


double
snowint(
	double	ppt,
	double	solrad,
	double	ithermf,
	double	tavg,
	double	ppt_intercept_coef,
	double	lai,
	double	albedo,
	double	tmin,
	double	u,
	double	tdew,
	double	delta_t)
{
    double snowin;
	double	canint;
	double	canrad;
	double	ea;
	double	cond;
	double	lerad;
	double	sec_day;
	double	netsol;
	double	nettherm;

    if (tavg > 0.0 )
	{
	snowin = 0.0;
	}
    else
	{
/*
  assume .2" max interception for full lodgepole canopy
  that's .5cm max and a .05cm/lai
  assume b(4) [ppt_intercept_coef]  = .001m/lai or  (.1 cm/lai) so it's
  1/2 the int. of water.
 
  lai snow storage: assume snow int.=rain intx2.0; Harding, 1986
*/
	canint =  (lai* ppt_intercept_coef * 2.0);
 
/* calculate available energy and sublimation */

	tavg = tavg+FREEZE;
	tmin = tmin+FREEZE;
	tdew = tdew+FREEZE;
	ea = sati(tdew);
	cond = 0.0;
	sec_day = delta_t*3600.0;
	netsol = ((1.0-albedo)*solrad)/sec_day;
	nettherm = (ithermf-(SNOW_EMISSIVITY*STEF_BOLTZ*pow(tmin,4.0)*86400.0))/86400.0; 
	canrad = netsol+nettherm;
	   canrad = MAX(canrad, 0.0);

        lerad = (sec_day*pm(tavg-FREEZE, ea, u, canrad, lai, cond))/LH_SUB(tdew);
	   lerad = (MAX(lerad, 0.0));
 
/*  potential interception is a minimum of intercepted snow and
  energy to sublimate snow.
*/
	if (canint > (lerad/1000.0)) {
	   canint = (lerad/1000.0);
	}

/* compute snowfall */

	snowin = ppt - canint;

/* if negative snowfall then set snowfall to 0.0; all intercepted */
 
	snowin = MAX(snowin, 0.0);
	}

    return(snowin);
}
