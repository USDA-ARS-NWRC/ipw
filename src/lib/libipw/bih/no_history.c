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
**	no_history -- turn off history mechanism in bihwrite
**
** SYNOPSIS
**	#include "bih.h"
**
**	void
**	no_history(
**		int             fd)	|* image file descriptor	 *|
**
** DESCRIPTION
**	no_history turns off the history mechanism.  If called for file
**	descriptor "fd" BEFORE bihwrite is called for the same file
**	descriptor, the normal printing of the history by bihwrite will
**	be suppressed.
**
**	no_history is normally called by programs (e.g. mux) that would
**	otherwise produce voluminous history records
**
** GLOBALS ACCESSED
**	_no_hist
*/

#include "ipw.h"
#include "_bih.h"

void
no_history(
	int             fd)		/* image file descriptor	 */
{
	ASSERT_OK_FD(fd);
	_no_hist[fd] = 1;
}
