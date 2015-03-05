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
**	lqhmake -- make an IPW LQH header
**
** SYNOPSIS
**	#include "lqh.h"
**
**	LQH_T *
**	lqhmake(
**		int             nbits,	  |* #bits / pixel		 *|
**		int             nbkpts,	  |* #breakpoints		 *|
**		pixel_t        *ival,	  |* array of integer bkpts	 *|
**		fpixel_t       *fval,	  |* array of floating-pt bkpts	 *|
**		char           *units,	  |* fpixel units of measurement *|
**		char           *interp)	  |* interpolation function name *|
**
** DESCRIPTION
**	lqhmake allocates a single IPW LQ header.  The header is initialized
**	with the arguments.
**
** RETURN VALUE
**	pointer to new LQ header; NULL if error
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_lqh.h"

LQH_T *
lqhmake(
	int             nbits,		/* #bits / pixel		 */
	int             nbkpts,		/* #breakpoints			 */
	pixel_t        *ival,		/* array of integer bkpts	 */
	fpixel_t       *fval,		/* array of floating-pt bkpts	 */
	char           *units,		/* fpixel units of measurement	 */
	char           *interp)		/* interpolation function name	 */
{
	LQH_T          *lqhp;		/* -> LQ header			 */
	int             bkpt;		/* current breakpoint #		 */

	assert(nbits > 0 && nbits <= sizeof(pixel_t) * CHAR_BIT);
	assert(nbkpts > 0);
	assert(ival != NULL);
	assert(fval != NULL);

 /*
  * allocate header
  */
 /* NOSTRICT */
	lqhp = (LQH_T *) hdralloc(1, sizeof(LQH_T), ERROR, LQH_HNAME);
	if (lqhp == NULL) {
		return (NULL);
	}

 /*
  * initialize scalars
  */
	lqhp->maplen = ipow2(nbits);
	lqhp->nbkpts = nbkpts;

 /*
  * initialize strings
  */
	if (units != NULL && (lqhp->units = strdup(units)) == NULL) {
		usrerr("insufficient memory");
		return (NULL);
	}

	if (interp != NULL && (lqhp->interp = strdup(interp)) == NULL) {
		usrerr("insufficient memory");
		return (NULL);
	}

 /*
  * initialize arrays
  */
	if (_lqharrays(lqhp) == ERROR) {
		return (NULL);
	}

	for (bkpt = 0; bkpt < nbkpts; ++bkpt) {
		int             i;	/* current integer bkpt	 */

		i = ival[bkpt];
		if (i < 0 || i >= lqhp->maplen) {
			usrerr("LQ header: bad integer breakpoint: %d", i);
			return (NULL);
		}

		lqhp->bkpt[bkpt] = i;
		lqhp->map[i] = fval[bkpt];
	}

	return (_lqhcheck(&lqhp, 1) ? lqhp : NULL);
}
