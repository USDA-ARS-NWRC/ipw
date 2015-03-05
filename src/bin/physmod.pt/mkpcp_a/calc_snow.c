/*
** NAME
**	calc_snow -- calculate % of precip that's snow and its density
**
** SYNOPSIS
**	#include "ipw.h"
**	#include "pgm.h"
**
**	void
**	calc_snow(
	    int  	 airtemp,        |* flag indicating that input temperature is air temperature (default is dew pt. temperature  *|
**	    double        T,		|* temperature		  *|
**	    double       *percent_snow,	|* % of precip's mass that's snow *|
**	    double       *rho_snow)	|* density of snow (kg/m^3)       *|
**
** DESCRIPTION
**	calc_snow determines the percentage of precipitation that's snow
**	{percent_snow} based on the air temperature {T_a}.  It also
**	computes the density of that snow {rho_snow}.
**
*/

#include "ipw.h"
#include "pgm.h"

void
calc_snow(
	int		airtemp,	/* air temperature flag		  */
	double	        T,		/* temperature input (dew pt is default) */
	double	       *percent_snow,	/* % of precip's mass that's snow */
	double	       *rho_snow)	/* density of snow (kg/m^3)       */
{
	if (airtemp) {
		if (T < -6.0) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 75.0;
		}
		else if (T < -3.5) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 100.0;
		}
		else if (T < -1.0) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 150.0;
		}
		else if (T < 1.0) {
			*percent_snow = (T - 3) / 4;	/* mixed rain & snow */
			*rho_snow = 200.0;
		}
		else if (T < 3.0) {
			*percent_snow = (T - 3) / 4;	/* mixed rain & snow */
			*rho_snow = 250.0;
		}
		else {
			*percent_snow = 0.0;	/* rain */
			*rho_snow = 0.0;
		}
	}
	else {
	
		if (T < -5.0) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 75.0;
		}
		else if (T < -3.0) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 100.0;
		}
		else if (T < -1.5) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 150.0;
		}
		else if (T < -0.5) {
			*percent_snow = 1.0;	/* snow */
			*rho_snow = 175.0;
		}
		else if (T < 0.0) {
			*percent_snow = 0.75;	/* wet snow */
			*rho_snow = 200.0;
		}
		else if (T < 0.5) {
			*percent_snow = 0.25;	/* mixed rain & snow */
			*rho_snow = 250.0;
		}
		else {
			*percent_snow = 0.0;	/* rain */
			*rho_snow = 0.0;
		}
	}
}
