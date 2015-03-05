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
**	delsv -- delete i'th string from strvec
**
** SYNOPSIS
**	#include "ipw.h"
**
**	STRVEC_T *
**	delsv(
**		REG_1 STRVEC_T *p,
**		REG_2 int       i)
**
** DESCRIPTION
**	delsv deletes the i'th string from strvec p.
**
**	There is no error indication if the i'th string doesn't exist -- delsv
**	just returns.
**
** RETURN VALUE
**	p
*/

#include "ipw.h"
#include "_strvec.h"

STRVEC_T *
delsv(
	REG_1 STRVEC_T *p,
	REG_2 int       i)
{
	assert(_ok_sv(p));
	assert(i >= 0);

 /*
  * if string to be deleted doesn't exist then we're already done
  */
	if (i >= p->n - 1 || p->v[i] == NULL) {
		return (p);
	}

 /*
  * de-allocate the string
  */
	(void) free(p->v[i]);

 /*
  * slide remaining strings toward origin of string vector
  */
	while ((p->v[i] = p->v[i + 1]) != NULL) {
		++i;
	}

 /*
  * if p->curr is now invalid then adjust it
  */
	if (p->curr > i) {
		p->curr = i;
	}

	return (p);
}
