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
**	geohspace -- get spacing from geodetic header
**
** SYNOPSIS
**	#include "geoh.h"
**
**	double
**	geohspace(
**		GEOH_T *geoh)
**
** DESCRIPTION
**	Geohspace returns grid spacing from geodetic header.
**
** RETURN VALUE
**	grid spacing	(if errno = 0)
*/

#include <math.h>
#include <errno.h>

#include "ipw.h"
#include "geoh.h"

#define TOL1		(1.e-2)
#define TOL2		(2 * FLT_EPSILON)

double
geohspace(
	GEOH_T         *geoh)		/* -> input header band	 */
{
	double          linespa;	/* line spacing			 */
	double          sampspa;	/* sample spacing		 */

	errno = 0;
 /*
  * spacing
  */
	linespa = geoh_dline(geoh);
	sampspa = geoh_dsamp(geoh);

 /*
  * make sure spacing same in both directions (adjust if close)
  */

	if (fabs((fabs(linespa) - fabs(sampspa)) / linespa) > TOL1) {
		usrerr("line spacing %g, samp spacing %g", linespa, sampspa);
		errno = ERROR;
		return (0.);
	}

	if (fabs((fabs(linespa) - fabs(sampspa)) / linespa) > TOL2) {
		warn("line spacing %g, samp spacing %g", linespa, sampspa);
	}

	return ((fabs(linespa) + fabs(sampspa)) / 2);
}
