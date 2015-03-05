/*
** NAME
**	units_as_str -- convert units id into a string
**
** SYNOPSIS
**	#include "units.h"
**
**	char *
**	units_as_str(
**		UNITS_T           id)	|* units id 			*|
**
** DESCRIPTION
**	units_as_str converts a units {id} into a string (usually for
**	printing).
**
** RETURN VALUE
**	units_as_str returns a string corresponding to the given units id.
**	If the units id is not valid, the string "unknown" is returned.
**
**	The pointer returned by this function points to static memory;
**	therefore, do not try free up (i.e., deallocate) this string.
**
** EXAMPLES
**	This function call
**
**		printf ("Error: Expected the units to be %s.",
**			units_as_str(U_METERS_PER_SECOND));
**
**	produces this output:
**
**		Error: Expected the units to be m/s.
**
** GLOBALS ACCESSED
**	_unitStrings
**
** HISTORY
**	Nov 95	Written by J. Domingo, OSU
*/

#include "ipw.h"
#include "units.h"
#include "_units.h"

char *
units_as_str(
	UNITS_T           id)		/* units id 			*/
{
	if (VALID_UNITS_ID(id))
		return _unitStrings[id];
	else
		return "unknown";
}
