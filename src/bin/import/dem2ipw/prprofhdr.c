/*
**	print out profile header
*/

#include "ipw.h"

#include "pgm.h"

void
prprofhdr(
	PROF_HDR       *ph)
{
	(void) fprintf(stderr,
		     "\n--------------------------------------------------");
	(void) fprintf(stderr, "\nParameters of DEM profile\n");
	(void) fprintf(stderr, "\nProfile number\t\t:\t(%d, %d)\n",
		       ph->rownum, ph->colnum);
	(void) fprintf(stderr, "Number of elevations\t:\t(%d, %d)\n",
		       ph->ni, ph->nj);
	(void) fprintf(stderr, "\nCoordinate of first point\t:\t%.10g\t%.10g\n",
		       ph->northing, ph->easting);
	(void) fprintf(stderr, "\nElevation datum\t\t:\t%8.2f\n",
		       ph->elevdatum);
	(void) fprintf(stderr, "Minimum elevation\t:\t%8.2f\n", ph->zmin);
	(void) fprintf(stderr, "Maximum elevation\t:\t%8.2f\n", ph->zmax);
}
