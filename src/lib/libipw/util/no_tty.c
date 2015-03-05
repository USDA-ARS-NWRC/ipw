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
**	no_tty -- exit if file descriptor is a terminal
**
** SYNOPSIS
**	#include "ipw.h"
**
**	void
**	no_tty(
**		int             fd)	|* file descriptor		 *|
**
** DESCRIPTION
**	If the argument file descriptor is connected to a terminal, then
**	no_tty calls either usage() (if the file descriptor is the standard
**	input) or error() (for all other cases).
**
**	Call no_tty to guarantee that an image file descriptor is not attached
**	to a terminal.
*/

#include <unistd.h>

#include "ipw.h"

void
no_tty(
	int             fd)		/* file descriptor		 */
{
	if (isatty(fd)) {
		if (fd == STDIN_FILENO) {
			usage();
		}

		if (fd == STDOUT_FILENO) {
			error("can't write image data to a terminal");
		}

		error("can't do image I/O on a terminal device");
	}
}
