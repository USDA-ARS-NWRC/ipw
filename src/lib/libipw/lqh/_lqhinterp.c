/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the Computer Systems Laboratory, University of
 * California, Santa Barbara and its contributors'' in the documentation
 * or other materials provided with the distribution and in all
 * advertising materials mentioning features or use of this software.
 *
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
** NAME
**	_lqhinterp -- interpolate LQ header breakpoint maps
**
** SYNOPSIS
**	#include "lqh.h"
**
**	int
**	_lqhinterp(
**		LQH_T   **lqhpp,	|* -> LQ header array		 *|
**		int       nbands)	|* # bands in *lqhpp		 *|
**
** DESCRIPTION
**	_lqhinterp interpolates the fpixel[pixel] mapping tables in the
**	specified LQ headers.  lqhpp points to an array of nbands pointers to
**	LQ headers.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** ERRORS
**	"{header}" header, band {band}: no "{function}" interpolation
**		The specified interpolation function is not supported.
*/

#include "ipw.h"
#include "lqh.h"

static int lqhlin(LQH_T *lqhp);

int
_lqhinterp(
	LQH_T         **lqhpp,		/* -> LQ header array		 */
	int             nbands)		/* # bands in *lqhpp		 */
{
	int             band;		/* current band #		 */
	int    (*ifunc)(LQH_T *llqhp);	/* -> interpolation function */

	assert(lqhpp != NULL);
	assert(nbands > 0);

	for (band = 0; band < nbands; ++band) {
		LQH_T          *lqhp;	/* lqhpp[band]			 */

		lqhp = lqhpp[band];
		if (lqhp == NULL) {
			continue;
		}

 /*
  * interpolate map
  */
		if (lqhp->mapped) {
			continue;
		}

		if (lqhp->interp == NULL ||
		    streq(lqhp->interp, LQH_LIN_INTERP)) {
			ifunc = lqhlin;
		}

 /*
  * NB: select alternate interpolation functions here
  */
		else {
			usrerr(
			   "\"%s\" header, band %d: no \"%s\" interpolation",
			       LQH_HNAME, band, lqhp->interp);
			return (ERROR);
		}

 /*
  * do the interpolation
  */
		if ((*ifunc) (lqhp) == ERROR) {
			return (ERROR);
		}

 /*
  * mark this header as already interpolated
  */
		lqhp->mapped = TRUE;
	}

 /*
  * done
  */
	return (OK);
}

/* ------------------------------------------------------------------------ */

static int
lqhlin(
	LQH_T          *lqhp)		/* -> LQ header			 */
{
	REG_3 int      *bkpt;		/* breakpoint array		 */
	REG_1 fpixel_t *map;		/* foo				 */
	REG_6 int       i;		/* breakpoint index		 */

	bkpt = lqhp->bkpt;
	map = lqhp->map;

	for (i = 1; i < lqhp->nbkpts; ++i) {
		REG_4 int       bp0;	/* lower breakpoint		 */
		REG_5 int       bp1;	/* upper breakpoint		 */

		bp0 = bkpt[i - 1];
		bp1 = bkpt[i];

		if (bp1 - bp0 > 1) {
			REG_2 int       j;	/* map index		 */
			FREG_1 fpixel_t kadd;	/* addend		 */
			FREG_2 fpixel_t kmul;	/* multiplier		 */
			FREG_3 fpixel_t ksub;	/* subtrahend		 */

 /*
  * the linear interpolation for bp0 < j < bp1 is:
  * 
  * map[j] = (j - bp0) / (bp1 - bp0) * (map[bp1] - map[bp0]) + map[bp0]
  */
			kadd = map[bp0];
			ksub = bp0;
			kmul = (map[bp1] - kadd) / (bp1 - ksub);

			for (j = bp0 + 1; j < bp1; ++j) {
				map[j] = (j - ksub) * kmul + kadd;
			}
		}
	}

 /*
  * Normally, the inverse mapping (fp->int) is done by binary-searching the
  * "map" array. In the special case where the inverse mapping is defined by a
  * single linear function (i.e. only 2 breakpoints), we stash the inverse
  * coefficients so they may be evaluated directly.  This should be faster
  * than the binary-search alternative, but of course it's NOT a general
  * solution.
  * 
  * The inverse mapping is:
  * 
  * i = (f - fmin)  / (fmax - fmin) * (imax - imin) + imin
  * 
  * imin may be elided since it is always 0.
  * 
  * The coefficients of the inverse transform are stashed in the "lininv" array.
  * The inverse mapping functions use these coefficients if the array exists.
  * The order of the coefficients is:
  * 
  * [0]	fmin
  * 
  * [1]	imax / (fmax - fmin)
  *
  * Comments from Dana Jacobsen 7/28/95:
  *
  * Actually, this isn't quite what happens.  What we want to be able to do
  * is to do something like:
  *
  *	 pixval = (fpval - lininv0) * lininv1;
  *
  * Now to do that, we need to be aware of two cases.  First, the normal case,
  * where breakpoint 0 is less than breakpoint 1, is exactly as described
  * above.  The second is what I call "reverse quantization", where the
  * small pixel value corresponds to the larger floating point value.  In this
  * case we don't do the above maps for [0] and [1], but instead do:
  *
  * [0] fmax
  *
  * [1] imax / (fmin - fmax)
  *
  * In fact, if we don't even worry about fmin and fmax, but just use the
  * breakpoints as given, we get the right affect.
  *
  * We also compute everything in double precision.  This leads to better
  * results.  We really ought to make a new fpixel_t type called, say,
  * flpixel_t for long floats (aka double), so with a simple change one could
  * move to double / long double.
  */
	if (lqhp->nbkpts == 2) {
		lqhp->lininv = (double *) ecalloc(2, sizeof(double));
		if (lqhp->lininv == NULL) {
			return (ERROR);
		}

		lqhp->lininv[0] = (double) lqhp->map[0];
		lqhp->lininv[1] = ((double) lqhp->maplen-1) /
		  ((double) lqhp->map[lqhp->maplen-1] - (double) lqhp->map[0]);
	}

	return (OK);
}
