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
**	addsv -- add a string to a strvec
**
** SYNOPSIS
**	#include "ipw.h"
**
**	STRVEC_T *
**	addsv(
**		REG_1 STRVEC_T *p,
**		char           *s)
**
** DESCRIPTION
**	addsv appends a copy of string s to strvec p.  If p is NULL, then a
**	new strvec is created with s as its first string.
**
**	addsv is used by the IPW image header ingestion routines to maintain
**	multi-valued string parameters.
**
** RETURN VALUE
**	p, or a pointer to a new strvec if p is NULL, or NULL if any memory
**	allocations fail.
*/

#include <string.h>

#include "ipw.h"
#include "_strvec.h"

STRVEC_T *
addsv(
	REG_1 STRVEC_T *p,
	char           *s)
{
	assert(s != NULL);

 /*
  * validate existing strvec
  */
	if (p != NULL) {
		assert(_ok_sv(p));
	}
 /*
  * allocate new strvec
  */
	else {
 /* NOSTRICT */
		p = (STRVEC_T *) ecalloc(1, sizeof(STRVEC_T));
		if (p == NULL) {
			return (NULL);
		}

		p->n = STRVEC_DSIZE;
		p->curr = 0;
 /* NOSTRICT */
		p->v = (char **) ecalloc(p->n, sizeof(char *));
		if (p->v == NULL) {
			return (NULL);
		}
	}
 /*
  * find first free slot in string vector
  */
	while (p->v[p->curr] != NULL) {
		++p->curr;
 /*
  * last slot MUST be NULL
  */
		if (p->curr >= p->n) {
			bug("corrupted string vector");
		}
	}

 /*
  * if only 1 slot left then grow the string vector
  */
	if (p->curr == p->n - 1) {
		p->n += STRVEC_DSIZE;
 /* NOSTRICT */
		p->v = (char **) realloc((char *) p->v,
					 (size_t) p->n * sizeof(char *));
		if (p->v == NULL) {
			return (NULL);
		}
	}

 /*
  * link copy of string into string vector
  */
	p->v[p->curr] = strdup(s);
	if (p->v[p->curr] == NULL) {
		return (NULL);
	}

 /*
  * add trailing NULL pointer
  */
	++p->curr;
	p->v[p->curr] = NULL;

	return (p);
}
