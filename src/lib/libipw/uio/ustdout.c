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
**	ustdout -- initialize UIO on standard output
**
** SYNOPSIS
**	int
**	ustdout(void)
**
** DESCRIPTION
**	ustdout initializes the UIO package for writing the program's standard
**	output.
**
**	ustdout should be called by an IPW main() if the program will be
**	writing image data to the standard output.  ustdout should NOT be
**	called if the program uses UNIX stdio output functions (e.g.,
**	printf).
**
**	ustdout causes program termination if any errors are encountered.
**
** RETURN VALUE
**	writable UNIX file descriptor
**
** GLOBALS ACCESSED
**
*/

#include <unistd.h>	/* for STDOUT_FILENO */

#include "ipw.h"
#include "_uio.h"

int
ustdout(void)
{
	if (_uioopen(STDOUT_FILENO, "(stdout)", UIO_WRITE) == ERROR) {
		error("can't initialize standard output");
	}

	return (STDOUT_FILENO);
}
