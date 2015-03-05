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
**	mnxfp -- minima and maxima of fpixel vectors
**
** SYNOPSIS
**	#include "fpio.h"
**
**	int
**	mnxfp(
**		fpixel_t   *x,		|* multiplexed vectors		 *|
**		int         npixv,	|* length of vectors		 *|
**		int         nbands,	|* # multiplexed bands		 *|
**		fpixel_t   *mmval)	|* min,max values		 *|
**
** DESCRIPTION
**	mnxfp finds the minima and maxima of the nbands interleaved
**	vectors of length npixv and of type fpixel_t.  The minima and
**	maxima of the bands are interleaved, i.e. the minimum and
**	maximum of band n are in mmval[2*n] and mmval[2*n+1].
**
**	mnxfp is normally used with the fpio package to find the
**	parameters necessary to write the floating-point pixels as
**	pixel_t's.
**
**	If the returned pointer "mmval" is used as an argument to lqhmake, it
**	must first be tested to see if the values are equal.  The following
**	code should follow a call to mnxfp that will be used this way:
**
**		if (parm.mmval[0] == parm.mmval[1]) {
**			parm.mmval[1] += 1;
**		}
**
** RETURN VALUE
**	OK (for backward compatibility)
*/

#include "ipw.h"
#include "fpio.h"

static void  mnx(fpixel_t *x, int npixv, int incr, fpixel_t *val);

int
mnxfp(
	fpixel_t       *x,		/* multiplexed vectors		 */
	int             npixv,		/* length of vectors		 */
	int             nbands,		/* # multiplexed bands		 */
	fpixel_t       *mmval)		/* min,max values		 */
{
	int             band;		/* band index			 */

	assert(npixv > 0);
	assert(nbands > 0);

	for (band = 0; band < nbands; ++band) {
		mnx(x, npixv, nbands, mmval);
		++x;
		mmval += 2;
	}
	return (OK);
}

/* ------------------------------------------------------------------------ */

 /*
  * min-max of single band
  */

static void
mnx(
	REG_1 fpixel_t *x,		/* interleaved vector		 */
	int             npixv,		/* length of vectors		 */
	int             incr,		/* increment betw elements	 */
	fpixel_t       *val)		/* min,max values (output)	 */
{
	REG_2 int       j;		/* loop index			 */
	FREG_1 fpixel_t min1;		/* local copy of minimum	 */
	FREG_2 fpixel_t max1;		/* local copy of maximum	 */

 /*
  * initialization, loops below go npixv-1 times
  */
	min1 = max1 = x[0];

 /*
  * if multiple bands are interleaved
  */
	if (incr != 1) {
		REG_3 int       i = incr;

		for (j = i * (npixv - 1); j > 0; j -= i) {
			if (min1 > x[j])
				min1 = x[j];
			else if (max1 < x[j])
				max1 = x[j];
		}
	}

 /*
  * else only one band
  */
	else {
		for (j = npixv; --j > 0;) {
			if (min1 > *++x)
				min1 = *x;
			else if (max1 < *x)
				max1 = *x;
		}
	}

 /*
  * move local copies into argument
  */
	val[0] = min1;
	val[1] = max1;
}
