/* 
	function rainint   calculate rain after removing intercepted amount
	 rain evaporates via Penman-Monteith method; rest returned to surface.

	output:	rain with interception evaporation removed

	input:	ppt		daily precipitation,
		tmin		minimum daily temperature, 
		tdew		daily dewpoint temperature,
		solrad		total daily incoming solar rad. (J),
		u		mean daily wind speed,
		intercept_coef	canopy interception coeficient, 
		lai		leaf area index (1 sided).
		delta_t		time period of Solin (hrs).

*/

#include <math.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "radiation.h"
#include "snow.h"



double
rainint(
	double	ppt,
	double	tavg,
	double	tdew,
	double	solrad,
	double	ithermf,
	double	u,
	double	intercept_coef,
	double	lai,
	double	delta_t)
{
	double	rainin;
	double	ea;
	double	cond;
	double	lerad;
	double	canint;
	double	sec_day;
	double	netsol;
	double	nettherm;
	double	netrad;

    if (tavg > 0.0 ) {

/* calculate canopy interception */

        canint = lai* intercept_coef;

/* calculate available energy terms and potential evaporation */

        tavg += FREEZE;
        tdew += FREEZE;
        ea = sati(tdew);
	cond = 0.0;
	sec_day = delta_t*3600.0;
	netsol = ((1.0-CANALB)*solrad)/sec_day;
	nettherm = (ithermf-(SNOW_EMISSIVITY*STEF_BOLTZ*pow(tavg,4.0)*86400.0))/86400.0;
	netrad = netsol+nettherm;
	   netrad = MAX(netrad, 0.0);
        lerad = (sec_day*pm(tavg-FREEZE, ea, u, netrad, lai, cond))/LH_VAP(tdew);
	lerad = (MAX(lerad, 0.0));

/* calculate actual evaporation */

	if (canint > (lerad/1000.0)) {
	   canint = (lerad/1000.0);
	}
	rainin =  ppt - canint;
	}
    else {
	rainin = 0.0;
	}

    if (rainin <= 0.0) {
	rainin = 0.0;
	}

    return(rainin);
}
