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
**	rmtrail -- remove trailing white space from string
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	rmtrail(
**		char *s)
**
** DESCRIPTION
**	Rmtrail writes an EOS after the last (i.e., rightmost) non-blank
**	character in "s".
*/

#include <ctype.h>
#include <string.h>

#include "ipw.h"

void
rmtrail(
	REG_1 char     *s)		/* -> string			 */
{
	REG_2 int       i;		/* loop counter			 */

	i = strlen(s);
	s += i;

	while (--i >= 0) {
		if (!isascii(*--s)) {
			break;
		}

		if (!isspace(*s)) {
			break;
		}

		*s = EOS;
	}
}
