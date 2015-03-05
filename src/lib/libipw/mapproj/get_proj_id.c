/*
** NAME
** 	get_proj_id -- get ID for projection's short name
** 
** SYNOPSIS
**	#include "mapproj.h"
**
**	int
**	get_proj_id(
**		char *	proj_name)	|* projection short name	*|
** 
** DESCRIPTION
** 	get_proj_id returns the projection ID for the given short name of
**	a geographic projection.  The list of recognized projection names
**	is:
**
**		Short Name	Projection's Full Name
**
**		albers		Albers Conical Equal Area
**		azmeqd		Azimuthal Equidistant
**		eqrect		Equirectangular
**		equidc		Equidistant Conic
**		geo		Geographic
**		gnomon		Gnomonic
**		gvnsp		General Vert. Near-Side Persp.
**		hom		Oblique Mercator (Hotine)
**		lamaz		Lambert Azimuthal Equal Area
**		lamcc		Lambert Conformal Conic
**		mercat		Mercator
**		miller		Miller Cylindrical
**		ortho		Orthographic
**		polyc		Polyconic
**		ps		Polar Stereographic
**		snsoid		Sinusoidal
**		som		Space Oblique Mercator (SOM)
**		spcs		State Plane Coordinates
**		stereo		Stereographic
**		tm		Transverse Mercator
**		utm		Universal Transverse Mercator
**		vgrint		Van der Grinten
** 
** RETURN VALUE
**	>=0		projection ID
**
**	UNKNOWN		invalid name was given
*/

#include "ipw.h"
#include "mapproj.h"

/* ------------------------------------------------------------------------ */

static char * proj_names[NPROJ] = {	/* names indexed by ID */
	"geo",
	"utm",
	"spcs",
	"albers",
	"lamcc",
	"mercat",
	"ps",
	"polyc",
	"equidc",
	"tm",
	"stereo",
	"lamaz",
	"azmeqd",
	"gnomon",
	"ortho",
	"gvnsp",
	"snsoid",
	"eqrect",
	"miller",
	"vgrint",
	"hom",
	"som"
};

/* ------------------------------------------------------------------------ */

int
get_proj_id(
	char *		proj_name)	/* projection short name	*/
{
	int		id;		/* projection id		*/

	assert(proj_name != NULL);

	for (id = 0; id < NPROJ; id++) {
		if(STREQ(proj_name, proj_names[id])) {
			return id;
		}
	}

	return UNKNOWN;
}
