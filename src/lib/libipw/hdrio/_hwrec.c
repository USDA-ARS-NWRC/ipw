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
**	_hwrec -- write an image header record
**
** SYNOPSIS
**	#include "_hdrio.h"
**
**	int
**	_hwrec(
**		int           fd,	|* input file descriptor	 *|
**		const char   *buf)	|* -> header record		 *|
**
** DESCRIPTION
**	_hwrec writes an image header record from buf to file descriptor fd.
**
**	The end of the header record must be delimited by a newline-EOS
**	sequence.
**
** RETURN VALUE
**	OK for success, ERROR for failure.
**
** GLOBALS ACCESSED
**
*/

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

int
_hwrec(
	int           fd,		/* input file descriptor	 */
	const char   *buf)		/* -> header record		 */
{
	if (uputs(fd, buf) == ERROR) {
		return (ERROR);
	}

	return (OK);
}
