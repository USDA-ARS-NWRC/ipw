/*
** NAME
**      check_range - checks if value within a specified range
**
** SYNOPSIS
**      void
**	check_range(
**		double	value,		|* value to check *|
**		double	min,		|* minimum acceptable value *|
**		double	max,		|* maximum acceptable value *|
**		char *	descrip)	|* description of value being checked *|
**
** DESCRIPTION
**	This routine checks if the value is within a specified range.
**	<descrip> contains a description of the value being checked.  The
**	range is between <min> and <max>.  If the value is out of range, 
**	prints an error message.
**
**	Note: This routine does not return if the value is out of range.
**
** HISTORY
**	Jun 96	Developed from the 'check_fval' routine of 'snobal'.
**		J. Domingo, OSU
*/

#include "ipw.h"
#include "pgm.h"

void
check_range(
	double	value,		/* value to check */
	double	min,		/* minimum acceptable value */
	double	max,		/* maximum acceptable value */
	char *	descrip)	/* description of value being checked */
{
	if (value < min || value > max) {
		error("%s (%f) out of range: %f to %f",
			descrip, value, min, max);
	}
}
