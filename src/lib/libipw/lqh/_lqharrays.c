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
**	_lqharrays -- allocate arrays for LQ header
**
** SYNOPSIS
**	#include "_lqh.h"
**
**	int
**	_lqharrays(
**		LQH_T    *lqhp)		|* -> LQ header			 *|
**
** DESCRIPTION
**	_lqharrays allocates the arrays required by the LQ header pointed to
**	by lqhp.
**
**	lqhp->maplen must be set before _lqharrays is called.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

#include "ipw.h"
#include "_lqh.h"

int
_lqharrays(
	LQH_T          *lqhp)		/* -> LQ header			 */
{
	assert(lqhp != NULL);
	assert(lqhp->maplen > 0);

 /* NOSTRICT */
	lqhp->bkpt = (int *) ecalloc(lqhp->maplen, sizeof(int));
	if (lqhp->bkpt == NULL) {
		return (ERROR);
	}
 /* NOSTRICT */
	lqhp->map = (fpixel_t *) ecalloc(lqhp->maplen, sizeof(fpixel_t));
	if (lqhp->map == NULL) {
		return (ERROR);
	}

	return (OK);
}
