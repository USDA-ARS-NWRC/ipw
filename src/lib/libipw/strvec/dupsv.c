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

#include <string.h>

#include "ipw.h"
#include "_strvec.h"

STRVEC_T *
dupsv(
	REG_1 STRVEC_T *p)
{
	REG_3 int       i;
	REG_2 STRVEC_T *newp;

 /*
  *  If given a NULL pointer, then just return NULL.  This allows for
  *  simpler code when duplicating image histories and annotations.
  */
	if (p == NULL)
		return NULL;

	assert(_ok_sv(p));

 /*
  * allocate new strvec
  */
 /* NOSTRICT */
	newp = (STRVEC_T *) ecalloc(1, sizeof(STRVEC_T));
	if (newp == NULL) {
		return (NULL);
	}

	newp->n = p->n;
	newp->curr = p->curr;

 /*
  * allocate string vector
  */
 /* NOSTRICT */
	newp->v = (char **) ecalloc(p->n, sizeof(char *));
	if (newp->v == NULL) {
		return (NULL);
	}

 /*
  * duplicate strings
  */
	for (i = 0; i < p->n && p->v[i] != NULL; ++i) {
		newp->v[i] = strdup(p->v[i]);
		if (newp->v[i] == NULL) {
			return (NULL);
		}
	}

	return (newp);
}
