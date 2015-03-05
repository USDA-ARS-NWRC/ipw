/*
 *  Array of strings for units ids defined in the include file "units.h"
 */

#include "ipw.h"
#include "units.h"
#include "_units.h"

char *_unitStrings[] = {
	"",		/* U_NONE			*/

	"%",		/* U_PERCENT			*/

	"C",		/* U_CELSIUS			*/
	"K",		/* U_KELVIN			*/
	"F",		/* U_FARENHEIT			*/

	"m",		/* U_METERS			*/
	"km",		/* U_KILOMETERS			*/

	"kg",		/* U_KILOGRAMS			*/

	"Pa",		/* U_PASCALS			*/

	"m/s",		/* U_METERS_PER_SECOND		*/

	"kg/m^2",	/* U_KILOGRAMS_PER_SQUARE_METER	*/
	"kg/m^3",	/* U_KILOGRAMS_PER_CUBIC_METER	*/
	"W/m^2",	/* U_WATTS_PER_SQUARE_METER	*/
	"J/m^2",	/* U_JOULES_PER_SQUARE_METER	*/

};
