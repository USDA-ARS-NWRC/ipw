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
**	_lqhcheck -- validate components of IPW LQH header
**
** SYNOPSIS
**	#include "_lqh.h"
**
**	bool_t
**	_lqhcheck(
**		LQH_T         **lqhpp,	|* -> array of -> LQH header	 *|
**		int             nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	_lqhcheck checks that lqhpp points to an array of nbands pointers
**	to valid LQH headers.
**
** RETURN VALUE
**	TRUE if lqhpp checks OK, else FALSE.
**
** HISTORY
**	4/14/95  modified compar function and qsort call, Dana Jacobsen, ERL-C.
*/

#include "ipw.h"
#include "_lqh.h"

/* ------------------------------------------------------------------------ */

static int
compar(
	const void * p1,
	const void * p2)
{
	return (*(const int *)p1 - *(const int *)p2);
}

/* ------------------------------------------------------------------------ */

bool_t
_lqhcheck(
	LQH_T         **lqhpp,		/* -> array of -> LQH header	 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	bool_t          found;		/* ? found at least 1 header	 */

	assert(lqhpp != NULL);

 /*
  * loop through possible bands
  */
	found = FALSE;

	for (band = 0; band < nbands; ++band) {
		int             i;	/* loop counter			 */
		LQH_T          *lqhp;	/* -> LQH for current band	 */
		fpixel_t        m;	/* slope of map		 */

		lqhp = lqhpp[band];
		if (lqhp == NULL) {
			continue;
		}

		found = TRUE;

		assert(lqhp->maplen > 0);
		assert(lqhp->bkpt != NULL);
		assert(lqhp->map != NULL);

		if (lqhp->nbkpts < 2) {
			usrerr("\"%s\" header, band %d: < 2 breakpoints",
			       LQH_HNAME, band);
			return (FALSE);
		}
 /*
  * sort breakpoints
  */
 /* NOSTRICT */
		(void) SORT_ALG((void *) lqhp->bkpt, (size_t) lqhp->nbkpts,
			     (size_t) sizeof(int), compar);

 /*
  * check for duplicate breakpoints
  */
		for (i = 1; i < lqhp->nbkpts; ++i) {
			if (lqhp->bkpt[i] == lqhp->bkpt[i - 1]) {
				usrerr(
				       "\"%s\" header, band %d: duplicate breakpoints",
				       LQH_HNAME, band);
				return (FALSE);
			}
		}

 /*
  * 0 and maplen-1 are implicit breakpoints
  */
		if (lqhp->bkpt[0] != 0) {
			assert(lqhp->nbkpts < lqhp->maplen);

			i = lqhp->nbkpts;
			do {
				lqhp->bkpt[i] = lqhp->bkpt[i - 1];
			} while (--i > 0);

			lqhp->bkpt[0] = 0;
			++lqhp->nbkpts;
		}

		if (lqhp->bkpt[lqhp->nbkpts - 1] != lqhp->maplen - 1) {
			assert(lqhp->nbkpts < lqhp->maplen);

			lqhp->bkpt[lqhp->nbkpts] = lqhp->maplen - 1;
			++lqhp->nbkpts;
		}

 /*
  * assure monotonic breakpoints
  */
		m = lqhp->map[lqhp->bkpt[1]] - lqhp->map[0];
		if (m == 0) {
			usrerr(
			 "\"%s\" header, band %d: non-monotonic breakpoints",
			       LQH_HNAME, band);
			return (FALSE);
		}

		for (i = 2; i < lqhp->nbkpts; ++i) {
			fpixel_t        curr_m;

			curr_m = lqhp->map[lqhp->bkpt[i]] -
				lqhp->map[lqhp->bkpt[i - 1]];

			if ((m > 0 && curr_m <= 0) || (m < 0 && curr_m >= 0)) {
				usrerr(
				       "\"%s\" header, band %d: non-monotonic breakpoints",
				       LQH_HNAME, band);
				return (FALSE);
			}
		}
	}

	assert(found);
 /*
  * done
  */
	return (TRUE);
}
