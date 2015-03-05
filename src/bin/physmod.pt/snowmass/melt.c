/*
	function melt	snowmelt
*/

#include <math.h>

#include "ipw.h"
#include "envphys.h"
#include "snow.h"
#include "radiation.h"
#include "pgm.h"

double
melt(
	double	snow_E_deficit,
	double	tavg,
	double	tmin,
	double	tdew,
	double	precip,
	double	solrad,
	double	ithermf,
	double	lai,
	double	albedo,
	double	snowpack,
	double	u,
	double	elev,
	double	delta_t)

{
	double	melt;
	double	floorad;
	double	ts;
	double	netsol;
	double	nettherm;
	double	netrad;
	double	radmelt;
	double	advecmelt;
	double	ea;
	double	e0;
	double	esat;
	double	hmelt;
	double	lemelt;
	double	press;
	double	T0;
	double	sec_day;


/*  RADIATION  MELT */


	if (solrad < 0.0) solrad = 0.0;
	if (ithermf < 0.0) ithermf = 0.0;

/* calculate solar radiation under vegetation canopy using Beer's law */

	floorad = solrad * exp(-0.5*lai);

/* convert temperatures to kelvin */

	tavg += FREEZE;
	ts = 0.0+FREEZE;
	tdew += FREEZE;

/* calculate vapor pressure of air (Pa) */

	ea = sati(tdew);

/* calculate air pressure */

	T0 = tavg-(elev*STD_LAPSE_M);
   	press = HYSTAT(SEA_LEVEL, T0, STD_LAPSE, (elev/1000.), GRAVITY, MOL_AIR);

/* compute seconds per day */

	sec_day = delta_t*3600.0;

/* calculate surface snowmelt */

    if (snow_E_deficit > 0.0) {

/* calculate surface vapor pressure assuming surface temp = 0.0 C */

        e0 = sati(ts);

/* calculate snowmelt */

	netsol = ((1.0-albedo)*floorad);

        nettherm = ithermf-(SNOW_EMISSIVITY*STEF_BOLTZ*pow(ts,4.0)*86400.0);

	netrad = netsol+nettherm;

	   netrad = MAX(netrad, 0.0);

	radmelt = netrad/LH_FUS(ts);

	advecmelt = (CP_WATER(tdew)*(precip*1000.)*(tdew-ts))/LH_FUS(ts);
	   advecmelt = MAX(advecmelt, 0.0);

/* assume h and le melt only occurs during day; Male and Gray p.390 */

	hmelt = (DH*u*(tavg-ts)*sec_day)/LH_FUS(ts); 
	   hmelt = MAX(hmelt, 0.0);

        lemelt = (DE*u*((ea-e0)/100.0)*sec_day)/LH_FUS(ts);

           lemelt = MAX(lemelt, 0.0);


	melt = (radmelt+advecmelt+hmelt+lemelt)/1000.0;
	   melt = MAX(melt, 0.0);
    }

/* else calculate sublimation if snow too cold to melt */

    else {	

/* calculate vapor pressure at tavg */

	 esat = sati(tavg); 

/* calculate sublimation from vapor pressure deficit */
/* assumes vpd approximates surface-atmosphere vapor pressure gradient */

           lemelt = ((DE*u*((esat-ea)/100.0)*sec_day)/LH_SUB(tdew))/1000.0;
 
	   melt = MAX(lemelt, 0.0);

     }


/* CORRECT FOR NEGATIVE SNOWMELT */

    if (melt > snowpack) {
	melt = snowpack;
       }

    return(melt);
}
