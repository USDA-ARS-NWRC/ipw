#define VK 0.41		/* von karmen const. */

#include <math.h>

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"

double
pm(
	double	ta,
	double	ea,
	double	u,
	double	Rn,
	double	lai,	/* projected (1-sided) LAI */
	double	cond,
	double	*radflx	,
	double	*vpdflx,
	double	*ra)
{
	double z0;
	double z;
	double rc;
	double arad;
	double gamma;
	double gammas;
	double slope;
	double penmon_rtn;
	double h0;
	double d0;
	double t1;
	double tak;
	double t2;
	double p;
	double xtrans;
	double vpd;
	double sat_vpres;
        double sat_vpres1;
        double sat_vpres2;


if (lai <= 1.0) {

/* calculate zero plane displacement, d0 */

        z0 = 0.01 * 1.0;

	h0 = 7.35 * z0;		/* Brutsaert p. 113 */

	d0 = h0 * (2/3);	/* Brutsaert p. 116 */

/* calculate aerodynamic resistance, ra (Stewart, 1984 p.6 ) */

	z = 2.0 * 1.0;		/* set measurement height of 2 m */

	*ra = (((log((z-d0)/z0))*(log((z-d0)/z0))) / (((VK*VK)*u)));

	arad = (Rn - (0.1 * Rn));	/* Campbell 1977, p. 138 */
}
	
else {

	*ra = 50.0 / lai;	/* Running BGC */
	arad = Rn * 1.0;		/* Campbell 1977, p. 138 */

}

/* convert temperature to kelvin */

	tak = ta+273.15;
	t1 = tak+0.5;
	t2 = tak-0.5;

/* calculate saturation vapor pressure */


        sat_vpres = sati(tak); 

	sat_vpres1 = sati(t1); 

	sat_vpres2 = sati(t2); 

/* calculate slope of saturation vapor pressure/temperature curve */

	slope = (sat_vpres1 - sat_vpres2) / 100;

/* calculate gamma */

	gamma = 0.646 + 0.0006 * ta;

/* calculate gammas */

   if (cond > 0.0) {

	rc = 1.0 / cond;
   }

   else {

	rc = 1.0*0.0;

   }

	gammas = (gamma * (*ra + rc)) / *ra;

/* calculate LE */

	vpd = (sat_vpres/100) - (ea/100);   /* vapor pressure deficit mb */

if(vpd < 0) {
	vpd = 0;
	}

	p = (1.292 - 0.00428 * ta);  /* air density, kg/m3 */

    xtrans = ((slope * (-1*(arad)) + ((p * CP_AIR * -1 * (vpd)) / *ra)) 
		/ (slope + gammas));

/* proportion of LE contributed by radiation term */

	*radflx = (slope * (-1*(arad))) / (slope + gammas);

/* proportion of LE contributed by the vapor pressure deficit */

	*vpdflx = ((p * CP_AIR * -1 * (vpd)) / *ra) / (slope + gammas);

	penmon_rtn = xtrans;

	return (penmon_rtn);
}
