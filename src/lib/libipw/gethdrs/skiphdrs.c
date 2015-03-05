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
**	skiphdrs -- skip IPW image headers
**
** SYNOPSIS
**	#include "gethdrs.h"
**
**	void
**	skiphdrs(
**		int      fd)		|* input image file descriptor	 *|
**
** DESCRIPTION
**	skiphdrs skips (i.e. reads and discards) all IPW image headers on
**	file descriptor fd.
**
**	skiphdrs is a simple way to skip all remaining headers on an input
**	image, once any desired headers have been ingested.
**
** ERRORS
**	can't skip {header-name} header
**	header read error
*/

#include "ipw.h"
#include "hdrio.h"
#include "gethdrs.h"

void
skiphdrs(
	int             fd)		/* input image file descriptor	 */
{
	REG_1 char     *hname;		/* name of current header	 */

	while ((hname = hrname(fd)) != NULL && strdiff(hname, BOIMAGE)) {
		if (hrskip(fd) == ERROR) {
			error("can't skip \"%s\" header", hname);
		}
	}

	if (hname == NULL) {
		error("header read error");
	}
}
