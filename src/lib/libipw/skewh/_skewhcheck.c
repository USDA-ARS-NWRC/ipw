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
**	skewhcheck -- validate components of IPW SKEWH header
**
** SYNOPSIS
**	#include "_skewh.h"
**
**	bool_t
**	_skewhcheck(
**		SKEWH_T    **skewhpp,	|* -> array of -> SKEWH header	 *|
**		int          nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	_skewhcheck checks that skewhpp points to an array of nbands pointers
**	to valid SKEWH headers.
**
** RETURN VALUE
**	TRUE if skewhpp checks OK, else FALSE.
*/

#include "ipw.h"
#include "skewh.h"
#include "_skewh.h"

bool_t
_skewhcheck(
	SKEWH_T       **skewhpp,	/* -> array of -> SKEWH header	 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	bool_t          found;		/* ? found at least 1 header	 */

	assert(skewhpp != NULL);
 /*
  * loop through possible bands
  */
	found = FALSE;

	for (band = 0; band < nbands; ++band) {
		SKEWH_T        *skewhp;	/* -> SKEWH for current band	 */

		skewhp = skewhpp[band];
		if (skewhp == NULL) {
			continue;
		}

		found = TRUE;

		if (skewhp->angle < SKEW_MIN || skewhp->angle > SKEW_MAX) {
			usrerr("\"%s\" header, band %d: %lf: bad angle",
			       SKEWH_HNAME, band, skewhp->angle);
			return (FALSE);
		}
	}

	assert(found);

	return (TRUE);
}
