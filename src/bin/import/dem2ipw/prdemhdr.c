/*
**	print DEM header
*/

#include "ipw.h"

#include "pgm.h"

void
prdemhdr(
	DEM_HDR        *dhdr)
{
	char           *run;
	char           *eun;
	int             j;

	(void) fprintf(stderr, "Parameters of digital elevation file");
	(void) fprintf(stderr, "\nName\t:\t%s\n", dhdr->title);
	(void) fprintf(stderr, "\nCoverage\t:\tCorner\tNorthing\t\tEasting\n");

	run = units(dhdr->refunits);

	for (j = 0; j < dhdr->polysides; j++) {
		(void) fprintf(stderr, "\t\t\t%4d\t%.1f\t%.1f\t%s\n",
			       j, dhdr->northing[j], dhdr->easting[j], run);
	}

	eun = units(dhdr->elevunits);
	(void) fprintf(stderr, "\nMinimum elevation\t:\t%5.0f\t%s\n",
		       dhdr->zmin, eun);
	(void) fprintf(stderr, "Maximum elevation\t:\t%5.0f\t%s\n",
		       dhdr->zmax, eun);

	(void) fprintf(stderr, "\nProfile direction\t:\t%g\tradians\n",
		       dhdr->profdir);

	(void) fprintf(stderr, "\nDEM level\t\t:\t%d\n", dhdr->demlevl);
	(void) fprintf(stderr, "DEM pattern\t\t:\t%d\n", dhdr->elevpatt);

	(void) fprintf(stderr, "Map projection\t\t:\t%s\n", proj(dhdr->refsys));
	(void) fprintf(stderr, "Zone\t\t\t:\t%d\n", dhdr->zone);

	run = units(dhdr->refunits);
	(void) fprintf(stderr, "\nEasting resolution\t:\t%6.2f\t%s\n",
		       dhdr->delx, run);
	(void) fprintf(stderr, "Northing resolution\t:\t%6.2f\t%s\n",
		       dhdr->dely, run);

	eun = units(dhdr->elevunits);
	(void) fprintf(stderr, "Elevation resolution\t:\t%6.2f\t%s\n",
		       dhdr->delz, eun);

	(void) fprintf(stderr, "\nAccuracy code\t\t:\t%4d\n", dhdr->accdata);
	(void) fprintf(stderr, "Rows of patches\t\t:\t%4d\n", dhdr->nrows);
	(void) fprintf(stderr, "Columns of patches\t:\t%4d\n", dhdr->ncols);
}
