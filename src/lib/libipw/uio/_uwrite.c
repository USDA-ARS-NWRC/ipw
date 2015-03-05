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
**	_write -- flush UIO input buffer
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int
**	_uwrite(
**	    UIO_T	*p)		|* -> UIO control block		*|
**
** DESCRIPTION
**	_uwrite writes the UIO buffer accessed via p to the associated file
**	descriptor.
**
** RETURN VALUE
**	OK	Write was successful
**
*	ERROR	An error occurred.
**
** GLOBALS ACCESSED
**
** ERRORS
**	"incomplete write"
**		some number of bytes less than the number requested were
**		written.  This is not necessarily due to a hard I/O error.
*/

#include <unistd.h>	/* for write() */

#include "ipw.h"
#include "_uio.h"

int
_uwrite(
	UIO_T		*p)		/* -> UIO control block		*/
{
	int		err;		/* write() return value		*/

	err = write(p->fd, (void *) p->buf, (size_t) p->nbytes);
	if (err != p->nbytes) {
		if (err == -1) {
			syserr();
		}
		else {
			usrerr("incomplete write");
		}

		uferr(p->fd);
		return (ERROR);
	}

	p->flags |= UIO_USED;

 /*
  * reset buffer state to "empty"
  */
	p->bufp = p->buf;
	p->nbytes = 0;

	return (OK);
}
