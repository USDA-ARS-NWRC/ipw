/*
** NAME
** 	baseflow -- runoff base flow function for simple water balance model
** 
** SYNOPSIS
**	float baseflow (bflow, sat)
**	float bflow;
**	float sat;
** 
** DESCRIPTION
** 	baseflow calculates the runoff base flow rate for the simple water
**	balance model.  Old method:  the given base flow factor is
**	simply returned as the actual base flow.  Current method:
**	the threshold baseflow is passed, and the actual base flow
**	is calculated as a function of percent saturation, scaled so that
**	the percent saturation can cause baseflow to vary above and below
**	a threshold value
** 
*/

#define		FACT	1.25
#define		MAX_SAT	1.0
#define		MIN_SAT	0.5

#include	"ipw.h"
#include	"pgm.h"

float
baseflow (
	float	bflowf,		/* threshold base flow (in mm) */
	float	ssat)		/* Percent soil saturation */
{
	if (ssat >= MAX_SAT)
		return (FACT * bflowf);

	else if (ssat < MIN_SAT)
		return (MIN_SAT * FACT * bflowf);

	else
		return (ssat * FACT * bflowf);
}
