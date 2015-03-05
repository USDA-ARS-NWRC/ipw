/*
** NAME
**	units_match -- check if a string matches a units id
**
** SYNOPSIS
**	#include "units.h"
**
**	int
**	units_match(
**		char	  *string,	|* string to test 		*|
**		UNITS_T    id)		|* units id 			*|
**
** DESCRIPTION
**	units_match determines if a given {string} matches a units {id}.
**	The string matches if it is the same string that the function
**	"units_as_str" returns for the given id.  units_match also
**	accepts variations for the string depending upon the id.
**
** RETURN VALUE
**	1 is returned if the string matches the id.  Otherwise, 0 is returned.
**
** EXAMPLES
**
**		units_match("mm", U_METERS)		returns 0
**		units_match("m", U_METERS)		returns 1
**		units_match("meters", U_METERS)		returns 1
**		units_match("Meters", U_METERS)		returns 1
**		units_match("METERS", U_METERS)		returns 1
**
** GLOBALS ACCESSED
**	_unitStrings
**
** HISTORY
**	Nov 95	Written by J. Domingo, OSU
*/

#include <string.h>

#include "ipw.h"
#include "units.h"
#include "_units.h"

int
units_match(
	char	         *string,	/* string to test 		*/
	UNITS_T           id)		/* units id 			*/
{
	assert(string != NULL);
	assert(VALID_UNITS_ID(id));

	if (STREQ(string, _unitStrings[id]))
		return 1;

	/*
	 *  Check for variations.
	 */
	switch (id) {

		case U_CELSIUS:
			if (strcasecmp(string, "CELSIUS") == 0)
				return 1;
			break;

		case U_KELVIN:
			if (strcasecmp(string, "KELVIN") == 0)
				return 1;
			break;

		case U_FARENHEIT:
			if (strcasecmp(string, "FARENHEIT") == 0)
				return 1;
			break;

		case U_METERS:
			if (strcasecmp(string, "METERS") == 0)
				return 1;
			break;

		case U_KILOMETERS:
			if (strcasecmp(string, "KILOMETERS") == 0)
				return 1;
			break;

		case U_KILOGRAMS:
			if (strcasecmp(string, "KILOGRAMS") == 0)
				return 1;
			break;

		case U_PASCALS:
			if (strcasecmp(string, "PASCALS") == 0)
				return 1;
			break;

		case U_KILOGRAMS_PER_SQUARE_METER:
			if (STREQ(string, "kg/m**2") == 0)
				return 1;
			break;

		case U_KILOGRAMS_PER_CUBIC_METER:
			if (STREQ(string, "kg/m**3") == 0)
				return 1;
			break;

		case U_WATTS_PER_SQUARE_METER:
			if (STREQ(string, "W/m**2") == 0)
				return 1;
			break;

		case U_JOULES_PER_SQUARE_METER:
			if (STREQ(string, "J/m**2") == 0)
				return 1;
			break;

		default:	/* No variations */
			break;

	} /* switch */

	return 0;
}
