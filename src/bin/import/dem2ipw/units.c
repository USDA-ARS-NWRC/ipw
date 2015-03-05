/*
** units for elevations and datum
*/

#include "ipw.h"

static char    *xun[4] = {
	"unknown",
	"feet",
	"meters",
	"seconds"
};

char           *
units(
	int             refunits)
{
	if (refunits < 1 || refunits > 3) {
		warn("units code = %d, unknown", refunits);
		refunits = 0;
	}
	return (xun[refunits]);
}
