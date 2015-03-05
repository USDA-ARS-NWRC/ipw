
/* Penman-Monteith estimation of latent energy flux */
/* for lai < 1, aerodynamic resistance calculated using a parameterized */
/* roughness length (i.e. grass and shrubs); for lai > 1, aerodynamic */
/* resistance assumed minimal and constant (i.e. over forests) */
/* (See Monteith and Unsworth, 1990; Running BGC) */

#define VK 0.41		/* von karmen const. */
/*
 *  NOTE:  The VON_KARMAN constant in physdefs.mac is 3.5e-1 ??
 */

#include	<math.h>

#include	"ipw.h"
#include	"envphys.h"
#include	"pgm.h"

double
pm(
	double ta, 	/* C */
	double ea,	/* Pa */
	double u,	/* m s^-1 */
	double Rn,	/* W m^-2 */
	double lai,
	double cond)	/* m s^-1 */
{
	double z0;	/* m */
	double z;	/* m */
	double rc;	/* s m^-1 */
	double arad;
	double gamma;
	double gammas;
	double slope;
	double h0;
	double d0;
	double t1;
	double tak;
	double t2;
	double p;
	double vpd;
	double sat_vpres;
        double sat_vpres1;
        double sat_vpres2;
	double vpdflx;
        double radflx;
	double leflux;
        double ra;	/* s m^-1 */


if (lai <= 1.0) {

/* calculate zero plane displacement, d0 */

        z0 = 0.01 * 1.0;

	h0 = 7.35 * z0;		/* Brutsaert p. 113 */

	d0 = h0 * (2./3.);	/* Brutsaert p. 116 */

/* calculate aerodynamic resistance, ra (Stewart, 1984 p.6 ) */

	z = 2.0 * 1.0;		/* set measurement height of 2 m */

	ra = (((log((z-d0)/z0))*(log((z-d0)/z0))) / (((VK*VK)*u)));

/* calculate available radiation Wm^-2 */
	arad = (Rn - (0.1 * Rn));	/* Campbell 1977, p. 138 */
}
	
else {

	ra = 50.0 / lai;	/* McNaughton and Black 1973 */

	arad = Rn * 1.0;		/* Campbell 1977, p. 138 */

}

/* convert temperature to kelvin */

	tak = ta+FREEZE;
	t1 = tak+0.5;
	t2 = tak-0.5;

/* calculate saturation vapor pressure */


        sat_vpres = sati(tak); 
	sat_vpres1 = sati(t1); 
	sat_vpres2 = sati(t2); 


/* calculate slope of saturation vapor pressure/temperature curve */

	slope = (sat_vpres1-sat_vpres2)/100.;

/* calculate gamma */

	gamma = 0.646+0.0006*ta;

/* calculate gammas */

   if (cond > 0.0) {
	rc = 1.0/cond;
   }
   else {
	rc = 1.0*0.0;
   }

	gammas = (gamma*(ra+rc))/ra;

/* calculate LE */

	vpd = (sat_vpres/100.)-(ea/100.);   /* vapor pressure deficit mb */

if(vpd < 0.0) {
	vpd = 0.;
	}

	p = (1.292-0.00428*ta);  /* air density, kg/m3 */


/* amount of LE contributed by radiation term */

	radflx = (slope*arad)/(slope+gammas);

/* amount of LE contributed by the vapor pressure deficit */

	vpdflx = ((p*CP_AIR*(vpd))/ra)/(slope+gammas);

/* flux away from surface is positive!! */

	leflux = (radflx+vpdflx);

/* returns W m^-2 */

	return (leflux);
}
