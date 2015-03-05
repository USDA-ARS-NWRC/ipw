/*
**	translate ascii profile header into variables
**	(Note: profile header stored in static; over-written each call.)
*/

#include "ipw.h"
#include <ctype.h>
#include "pgm.h"

static PROF_HDR _ph;
static int      counter;
static int      already = 0;

PROF_HDR       *
profhdr(
	DEM_SAVHDR     *s)		/* saved part of profile header	 */
{
	B_RECORD        zot;
	B_RECORD       *rb;

	PROF_HDR       *ph;

 /*
  * read profile header
  */
	rb = &zot;
	(void) fscanf(parm.i_fp, "%s", rb->rowid);
	(void) fscanf(parm.i_fp, "%s", rb->colid);
	(void) fscanf(parm.i_fp, "%s", rb->nrows);
	(void) fscanf(parm.i_fp, "%s", rb->ncols);
	(void) fscanf(parm.i_fp, "%s", rb->xgo);
	(void) fscanf(parm.i_fp, "%s", rb->ygo);
	(void) fscanf(parm.i_fp, "%s", rb->elevdatum);
	(void) fscanf(parm.i_fp, "%s", rb->elevmin);
	(void) fscanf(parm.i_fp, "%s", rb->elevmax);
 /*
  * row and column number of this profile
  */
	ph = &_ph;
	ph->rownum = atoid(rb->rowid);
	ph->colnum = atoid(rb->colid);
	if (ph->rownum > 1 && ph->colnum > 1) {
		error("rownum = %d, colnum = %d (one must = 1)",
		      ph->rownum, ph->colnum);
	}

 /*
  * on first pass, save starting profile number; otherwise keep track
  */
	if (!already) {
		already = 1;
		counter = s->num = MAX(ph->rownum, ph->colnum);
	}
	else if ((s->num = MAX(ph->rownum, ph->colnum)) != ++counter) {
		warn("inconsistent profile count, profile %d", s->num);
	}
 /*
  * # elevations in this profile
  */
	ph->ni = atoid(rb->nrows);
	ph->nj = atoid(rb->ncols);
	if (ph->ni > 1 && ph->nj > 1) {
		error("ni = %d, nj = %d (one must = 1)",
		      ph->ni, ph->nj);
	}
	s->nelev = MAX(ph->ni, ph->nj);
 /*
  * planimetric coordinates of starting pt in this profile (reversed in
  * original record)
  */
	s->northing = ph->northing = bigatof(rb->ygo);
	s->easting = ph->easting = bigatof(rb->xgo);
 /*
  * elevation datum for this profile
  */
	ph->elevdatum = bigatof(rb->elevdatum);

 /*
  * minimum and maximum elevations in this profile
  */
	ph->zmin = bigatof(rb->elevmin);
	ph->zmax = bigatof(rb->elevmax);
	if (ph->zmin < parm.zmin) {
		warn("profile %d, min elev %g < min for file %g",
		     s->num, ph->zmin, parm.zmin);
	}
	if (ph->zmax > parm.zmax) {
		warn("profile %d, max elev %g > max for file %g",
		     s->num, ph->zmax, parm.zmax);
	}
	return (ph);
}
