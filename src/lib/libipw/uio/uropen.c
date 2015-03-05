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
**	uropen -- open UNIX file for reading
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	uropen(
**		const char      *name)	|* file name			 *|
**
** DESCRIPTION
**	uropen opens the file name for reading.  The name "-" is understood to
**	mean the standard input.
**
**	Use uropen wherever you would normally use the UNIX system call open()
**	to open a file for reading.
**
** RETURN VALUE
**	>= 0	A readable UNIX file descriptor for successful open.
**
**	ERROR	An error occurred.
**
** GLOBALS ACCESSED
*/

#include <fcntl.h>

#include "ipw.h"
#include "_uio.h"

int
uropen(
	const char     *name)		/* file name			 */
{
	int             fd;		/* file descriptor		 */

	assert(name != NULL);

	if (streq(name, "-")) {
		return (ustdin());
	}

	fd = open(name, O_RDONLY);
	if (fd == -1) {
		syserr();
		return (ERROR);
	}

	if (_uioopen(fd, name, UIO_READ) == ERROR) {
		return (ERROR);
	}

	return (fd);
}
