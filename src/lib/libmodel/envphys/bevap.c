#include	"ipw.h"
#include	"envphys.h"

double
bevap(
	double	netrad,	  /* net allwave radiation (W/m^2)  */
	double	advec,	  /* advected heat (W/m^2)	    */
	double	bowen,	  /* Bowen ratio (H/LE)		    */
	double	storage,  /* change in heat storage (W/m^2) */
	double	ts)	  /* surface temperature (K)	    */
{
	double 	le, rhoe, rate;

	/*	le = latent heat of vaporization at ts (J/kg)	*/
	/*	rhoe = density of liquid water at 25 C (kg/m^3)	*/

	le = LH_VAP(ts);

	rhoe = 997.07;

	rate = (netrad + advec + storage)/(rhoe * le * (1.0 + bowen));

	return(rate);
}
