/*
**	translate ascii header into entries in DEM_HDR
*/

#include <ctype.h>
#include <math.h>
#include <string.h>

#include "ipw.h"

#include "pgm.h"

void
demhdr(void)
{
	A_RECORD        zot;
	A_RECORD       *ra;
	DEM_HDR        *dhdr;
	int             j;
	register char  *p;

 /* NOSTRICT */
	dhdr = (DEM_HDR *) ecalloc(1, sizeof(DEM_HDR));
	assert(dhdr != NULL);

 /* read in Record A */
	ra = &zot;
 /* NOSTRICT */
	if (fread((char *) ra, sizeof(A_RECORD), 1, parm.i_fp) != 1) {
		error("read error, Record A");
	}

 /*
  * copy title and remove trailing blanks
  */
	(void) memcpy(dhdr->title, ra->filename, TSIZE);
	p = dhdr->title + TSIZE - 1;
	*p = '\0';
	p--;
	while (isspace(*p)) {
		*p-- = '\0';
	}
 /* replace any non-ascii characters */
	p = dhdr->title - 1;
	while (*++p != '\0') {
		if (!isascii(*p))
			*p = toascii(*p);
	}
 /*
  * DEM level code
  */
	dhdr->demlevl = atoid(ra->levlcode);
	if (dhdr->demlevl < 1 || dhdr->demlevl > 3) {
		warn("DEM level code = %d, out of range", dhdr->demlevl);
	}
 /*
  * elevation code
  */
	dhdr->elevpatt = atoid(ra->elevpatt);
	if (dhdr->elevpatt < 1 || dhdr->elevpatt > 2) {
		warn("elevation pattern code = %d, out of range",
		     dhdr->elevpatt);
	}
 /*
  * planimetric reference system
  */
	dhdr->refsys = atoid(ra->refsys);
	if (dhdr->refsys < 0 || dhdr->refsys > 20) {
		warn("DEM reference system code = %d, out of range",
		     dhdr->refsys);
	}
 /*
  * zone
  */
	dhdr->zone = atoid(ra->zone);
 /*
  * map projection parameters, ignored if 0 <= refsys <= 2
  */
	if (dhdr->refsys >= 0 || dhdr->refsys <= 2) {
		for (j = 0; j < 15; j++)
			dhdr->tcons[j] = 0;
	}
	else {
		for (j = 0; j < 15; j++)
			dhdr->tcons[j] = bigatof(&(ra->proj_param[j][0]));
	}

 /*
  * units of measure for reference system
  */
	dhdr->refunits = atoid(ra->refunits);
	if (dhdr->refunits < 0 || dhdr->refunits > 3) {
		warn("reference units code = %d, out of range",
		     dhdr->refunits);
	}
 /*
  * units of measure for elevations
  */
	dhdr->elevunits = atoid(ra->elevunits);
	if (dhdr->elevunits < 1 || dhdr->elevunits > 2) {
		warn("elevation units code = %d, out of range",
		     dhdr->elevunits);
	}

 /*
  * number of sides of polygon
  */
	if ((dhdr->polysides = atoid(ra->polysides)) != 4) {
		warn("# polygon sides = %d, not supported",
		     dhdr->polysides);
	}

 /*
  * coordinates of sides of polygon (initially set min and max to
  * middle)
  */
	parm.nmin = parm.emin = 0;
	for (j = 0; j < 4; j++) {
		parm.emin +=
			(dhdr->easting[j] = bigatof(&(ra->corners[j][0][0])));
		parm.nmin +=
			(dhdr->northing[j] = bigatof(&(ra->corners[j][1][0])));
	}
	parm.emin /= 4;
	parm.nmin /= 4;
	parm.emax = parm.emin;
	parm.nmax = parm.nmin;

 /*
  * minimum and maximum elevations
  */
	dhdr->zmin = bigatof(ra->elevmin);
	dhdr->zmax = bigatof(ra->elevmax);
	parm.zmin = floor(dhdr->zmin);
	parm.zmax = ceil(dhdr->zmax);
	if (parm.zmin < 0 && parm.raw) {
		error("minimum elevation negative (%g), %s", parm.zmin,
		      "therefore can't use -r option");
	}

 /*
  * rotation angle
  */
	dhdr->profdir = bigatof(ra->rotate);

 /*
  * accuracy code
  */
	dhdr->accdata = atoid(ra->acc_code);
 /*
  * resolution in x-, y-, and z-directions
  */
	dhdr->delx = litatof(ra->delx);
	dhdr->dely = litatof(ra->dely);
	dhdr->delz = litatof(ra->delz);
 /*
  * rows and cols of profiles
  */
	dhdr->nrows = atoid(ra->rownum);
	dhdr->ncols = atoid(ra->colnum);
	if (dhdr->nrows > 1 && dhdr->ncols > 1) {
		warn("rows of profiles and cols of profiles both > 1");
	}
	dhdr->nprofls = MAX(dhdr->nrows, dhdr->ncols);
 /*
  * values to save in parameter structure
  */
	parm.nprofls = dhdr->nprofls;
	parm.dely = dhdr->dely;
	parm.delx = dhdr->delx;
	parm.units = units(dhdr->elevunits);
	parm.runits = units(dhdr->refunits);
	parm.csys = proj(dhdr->refsys);
 /*
  * print header
  */
	if (parm.verbose) {
		prdemhdr(dhdr);
	}
}
