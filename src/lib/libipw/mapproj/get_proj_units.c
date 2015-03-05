/*
** NAME
** 	get_proj_units -- get name for projection units ID
** 
** SYNOPSIS
**	#include "mapproj.h"
**
**	char *
**	get_proj_units(
**		int	units_id)	|* projection units ID		 *|
** 
** DESCRIPTION
** 	get_proj_units returns the name of the geographic projection units
**	that corresponds to the given units ID.  If an invalid ID is
**	specified, the name "(unknown)" is returned.
** 
** RESTRICTIONS
**	The returned character string is in static memory; so do NOT
**	attempt to "free" it up.
** 
** RETURN VALUE
**	pointer to name of projection units
*/

#include "ipw.h"
#include "mapproj.h"

/* ------------------------------------------------------------------------ */

static char *unit_names[] = {
	"radians",
	"feet",
	"meters",
	"seconds",
	"degrees",
	"dms"
};

/* ------------------------------------------------------------------------ */

char *
get_proj_units(
	int		units_id)	/* projection units ID		 */
{
	if (units_id < 0 || units_id >= NUNITS) {
		return "(unknown)";
	}

	return unit_names[units_id];
}
