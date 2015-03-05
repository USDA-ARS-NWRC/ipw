/*
** NAME
** 	aet -- actual evapotranspiratin function for simple water balance model
** 
** SYNOPSIS
**	float aet (pet, sat)
**	float pet;
**	float sat;
** 
** DESCRIPTION
** 	aet calculates the actual evapotranspiration rate for the simple water
**	balance model.  Current method:  the potential evaportanspiration (pet)
**	scaled to percent soil moisture saturation, such that at 100% saturation
**	pet = et; but at sat < 100% the proportion of pet assigned to et is
**	set as an exponential decay function:
**
**		aet = pet * exp ((sat * k) - k)
**
**	where soil saturation (sat) varies from 0.0 to 1.0, and k is around 5.0.
** 
*/

#define		KFACT	3.0	/* exponential decay factor */
#define		MAX_SAT	1.0	/* max soil sat for aet == pet */
#define		MIN_SAT	0.0	/* min soil sat for aet == min_et (usually 0) */

#include	<math.h>
#include	"ipw.h"
#include	"pgm.h"

float
aet (
	float	pet,		/* Potential evapotranspiration (in mm) */
	float	ssat)		/* Percent soil saturation */
{
	if (ssat >= MAX_SAT)
		return (MAX_SAT * pet);

	else if (ssat <= MIN_SAT)
		return (MIN_SAT * pet);

	else
		return ((float) ( pet * exp((ssat * KFACT) - KFACT)));

}
