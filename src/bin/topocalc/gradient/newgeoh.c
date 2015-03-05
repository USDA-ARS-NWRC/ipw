/*
 * create new window header and get spacing from old
 */

#include <math.h>

#include "ipw.h"
#include "fpio.h"
#include "geoh.h"
#include "hdrio.h"

#define TOL1		(1.e-2)
#define TOL2		(2 * FLT_EPSILON)

GEOH_T        **
newgeoh(
	int             nbands,		/* # bands in output image	 */
	int             fdo,		/* output file desc		 */
	GEOH_T        **i_geoh,		/* -> input geodetic header	 */
	fpixel_t       *spacing)	/* grid spacing (m)		 */
{
	double          linespa;	/* line spacing			 */
	double          sampspa;	/* sample spacing		 */
	GEOH_T        **o_geoh;		/* -> output geodetic header	 */
	int             j;

 /*
  * make sure units are meters
  */
	if (strdiffn(geoh_units(i_geoh[0]), "m", 1)) {
		warn("input units \"%s\", should be \"m\"",
		     geoh_units(i_geoh[0]));
	}
 /*
  * don't change spacing if already specified
  */
	if (spacing[0] != 0 && spacing[1] != 0) {
		warn("spacing in geodetic header ignored");
	}
	else {
		linespa = geoh_dline(i_geoh[0]);
		sampspa = geoh_dsamp(i_geoh[0]);
		spacing[0] = fabs(linespa);
		spacing[1] = fabs(sampspa);
	}
 /*
  * make new geodetic header, copy of old
  */
 /* NOSTRICT */
	o_geoh = (GEOH_T **) hdralloc(nbands, sizeof(GEOH_T *), fdo,
				      GEOH_HNAME);
	if (o_geoh != NULL) {
		for (j = 0; j < nbands; ++j) {
			o_geoh[j] = i_geoh[0];
		}
	}

	return (o_geoh);
}
