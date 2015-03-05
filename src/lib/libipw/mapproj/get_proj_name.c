/*
** NAME
** 	get_proj_name -- get name of projection for given ID
** 
** SYNOPSIS
**	#include "mapproj.h"
**
**	char *
**	get_proj_name(
**		int	proj_id)	|* projection ID		 *|
** 
** DESCRIPTION
** 	get_proj_name returns the name of the geographic projection that
**	corresponds to the given projection ID.  If the ID is not valid,
**	the name "(unknown)" is returned.
** 
** RESTRICTIONS
**	The returned character string is in static memory, so do NOT
**	attempt to "free" it.
** 
** RETURN VALUE
**	pointer to character string containing projection name
*/

#include "ipw.h"
#include "mapproj.h"

/* ------------------------------------------------------------------------ */

static char * proj_names[NPROJ] = {
	"Geographic",
	"Universal Transverse Mercator",
	"State Plane Coordinates",
	"Albers Conical Equal Area",
	"Lambert Conformal Conic",
	"Mercator",
	"Polar Stereographic",
	"Polyconic",
	"Equidistant Conic",
	"Transverse Mercator",
	"Stereographic",
	"Lambert Azimuthal Equal Area",
	"Azimuthal Equidistant",
	"Gnomonic",
	"Orthographic",
	"General Vert. Near-Side Persp.",
	"Sinusoidal",
	"Equirectangular",
	"Miller Cylindrical",
	"Van der Grinten",
	"Oblique Mercator (Hotine)",
	"Space Oblique Mercator (SOM)"
};

/* ------------------------------------------------------------------------ */

char *
get_proj_name(
	int		proj_id)	/* projection ID		 */
{
	if(proj_id < 0 || proj_id >= NPROJ)
		return "(unknown)";

	return proj_names[proj_id];
}
