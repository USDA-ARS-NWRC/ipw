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
**	uputs -- write a string to a UNIX file descriptor
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	uputs(
**		int             fd,	|* input file descriptor	 *|
**		const char     *buf)	|* -> input buffer		 *|
**
** DESCRIPTION
**	uputs writes an EOS-terminated string from buf to fd.  The trailing
**	EOS is not written.
**
**	Use uputs for efficient output of ASCII text (e.g., image headers)
**	to IPW data files.
**
** RETURN VALUE
**	>= 0	number of characters written
**
**	ERROR	errors or end-of-file.
**
** GLOBALS ACCESSED
**
*/

#include <string.h>

#include "ipw.h"

int
uputs(
	int             fd,		/* input file descriptor	 */
	const char     *buf)		/* -> input buffer		 */
{
	int             nbytes;		/* #bytes to read		 */

	assert(buf != NULL);

	nbytes = strlen(buf);
	if (nbytes > 0) {
		nbytes = uwrite(fd, (const addr_t) buf, nbytes);
	}

	return (nbytes);
}
