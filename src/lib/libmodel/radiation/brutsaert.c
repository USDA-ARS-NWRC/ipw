#include <math.h>

#include "ipw.h"
#include "radiation.h"
#include "envphys.h"

double 
brutsaert(
	double   ta,		/* air temp (K)				*/
	double   lambda,	/* temperature lapse rate (deg/m)	*/
	double   ea,		/* vapor pressure (Pa)			*/
	double   z,		/* elevation (z)			*/
	double   pa)		/* air pressure (Pa)			*/
{
	double 	t_prime;
	double 	rh;
	double 	e_prime;
	double 	air_emiss;

	t_prime = ta - (lambda * z);
	rh = ea / sati(ta);
	if (rh > 1.0) {
		if (rh > 1.02)
			warn("brutseart: ea=%lg > sat; RH set to 1.0", ea);
		rh = 1.0;
	}
	e_prime = (rh * sati(t_prime))/100.0;
/*	e_prime = rh * sati(t_prime);	*/

	air_emiss = (1.24*pow((e_prime/t_prime), 1./7.))*pa/SEA_LEVEL;
/* "if" statement below is new */
	if (air_emiss > 1.0) {
                air_emiss = 1.0;
        }

	return(air_emiss);
}
