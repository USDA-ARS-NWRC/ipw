#define VK 0.41		/* von karmen const. (Monteith and Unsworth 1990) */

#include	<math.h>
#include	"ipw.h"
#include 	"envphys.h"

double
pm(
	double ts,
	double ta,
	double ea,
	double u,
	double Rn,
	double G,
	double z0,
	double z,
	double rc,
	double *ratio,
	double *radflx,
	double *vpdflx,
	double *ra,
	double *Rib,
	double *F)
{
	double alpha;
	double zed;
	double gamma;
	double gammas;
	double slope;	/* slope of saturation vp-temperature curve, mb */
	double es;
	double ea1;
	double penmon_rtn;
	double h0;	/* Mean roughness height, m */
	double d0;	/* zero plane displacement, m */
	double t1;
	double t2;
	double p;	/* air density, kg m^-3 */
	double sati();	/* returns saturation vapor pressure, Pa, at given temp(k) */
	double xtrans;
	double vpd;	/* vapor pressure deficit, */

/* calculate zero plane displacement, d0 */

   /* Brutsaert p. 113 */
	h0 = 7.35 * z0;		

   /* Brutsaert p. 116 */
	d0 = h0 * (2.0/3.0);

/* calculate aerodynamic resistance, ra (Stewart, 1984 p.6 ) */

/* calculate bulk richardson #,(Mahrt and Ek, 1984) */

	*Rib = (GRAVITY*((ta+FREEZE)-(ts+FREEZE))*z)/((((ts+FREEZE)+(ta+FREEZE))/2.0)*(u*u));

/* calculate stability function, F, and aerodynamic resistance, ra */

	if (*Rib > 0.0) {

	   *F = exp(*Rib);


	   *ra = (((log((z-d0)/z0))*(log((z-d0)/z0))) / (((VK*VK)*u)* *F));
	}
	
	else if (*Rib < 0.0) {

	   zed = ((-1 * *Rib * (z-d0))/z0);

	   alpha = (VK*VK)/((log((z-d0)/z0))*(log((z-d0)/z0)));

	   *F = 1-((15 * *Rib)/(1+(75*((VK*VK)/alpha)*pow(zed,0.5))));

	   *ra = (((log((z-d0)/z0))*(log((z-d0)/z0))) / (((VK*VK)*u) / *F));

	}

	else {

	  *ra = (((log((z-d0)/z0))*(log((z-d0)/z0))) / ((VK*VK)*u));
	}


/* convert temperature to kelvin */

	t1 = ((ta+0.5)+FREEZE);
	t2 = ((ta-0.5)+FREEZE);

/* calculate slope of saturation vapor pressure/temperature curve */

	slope = (sati(t1) - sati(t2)) / 100.0;

/* calculate gamma (Running and Coughlan 1988) */

	gamma = 0.646 + 0.0006 * ta;

/* calculate gammas */

	gammas = (gamma * (*ra + rc)) / *ra;

/* calculate LE */

   /* air vapor pressure, mb */
	ea1 = ea / 100.0;	

   /* saturation vapor pressure, mb */
es = sati((ta + FREEZE)) / 100.0;   

vpd = es - ea1;

if(vpd < 0.0) {
	vpd = 0.0;
	}

	p = (1.292 - 0.00428 * ta);  /* air density, kg/m^3 */

    xtrans = ((slope * (-1.0*(Rn - G)) + ((p * CP_AIR * -1.0 * (vpd)) / *ra)) 
		/ (slope + gammas));

/* proportion of LE contributed by radiation term */

	*radflx = (slope * (-1*(Rn - G))) / (slope + gammas);

/* proportion of LE contributed by the vapor pressure defecit */

	*vpdflx = ((p * CP_AIR * -1.0 * (vpd)) / *ra) / (slope + gammas);

/* calculate ratio of radiation to v.pressure deficit terms of le */

	*ratio = fabs(*vpdflx / *radflx);

	penmon_rtn = xtrans;

	return (penmon_rtn);
}
