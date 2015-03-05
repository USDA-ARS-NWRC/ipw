/*
** projection system
*/

#include "ipw.h"

static char    *xproj[4] = {
	"geographic",
	"UTM",
	"State Plane",
	"unknown"
};

char           *
proj(
	int             refsys)
{
	if (refsys < 0 || refsys > 2) {
		warn("projection system = %d, unknown", refsys);
		refsys = 3;
	}
	return (xproj[refsys]);
}
