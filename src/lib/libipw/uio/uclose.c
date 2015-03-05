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
**	uclose -- close IPW file descriptor
**
** SYNOPSIS
**	int
**	uclose(
**		int             fd)	    |* file descriptor		 *|
**
** DESCRIPTION
**	uclose closes an IPW file descriptor and clears the associated UIO
**	control block.  If the descriptor refers to an output file, any
**	pending buffered output is first flushed.
**
** RETURN VALUE
**	OK	The file was closed succesfully.
**
**	ERROR	An error occurred.
**
** GLOBAL VARIABLES MODIFIED
**	_uiocb[fd]	UIO control block for file descriptor fd
**
*/

#include <strings.h>
#include <unistd.h>	/* for STDIN_FILENO */

#include "ipw.h"
#include "_uio.h"

int
uclose(
	int             fd)		/* file descriptor		 */
{
	UIO_T          *p;		/* -> UIO control block		 */
	int		i;

	ASSERT_OK_FD(fd);
	p = &_uiocb[fd];

 /*
  * remove file desc from list of open files
  */
	for (i = 0; i < _ipwnumfds; i++)
	    if (_ipwfds[i] == fd) {
		/*
		 *  Found file descriptor, so move remaining descriptors
		 *  one spot to the left (overwriting the given fd).
		 */
		for ( ; i < _ipwnumfds - 1; i++) {
		    _ipwfds[i] = _ipwfds[i+1];
		}
		_ipwnumfds--;
		break;
	    }

 /*
  * if already closed then do nothing
  */
	if (p->flags == 0) {
		return OK;
	}

 /*
  * check to see if this is the right close to be calling
  */

	ASSERT_IO_LEVEL(FTYPE_UIO, p->level);

 /*
  * flush pending output
  *
  * NB: should we drain input too?
  */
	if (p->flags & UIO_WRITE) {
	    if (uwflush(fd) == ERROR) {
		return ERROR;
	    }
	}

 /*
  * do a UNIX close (unless standard input)
  */
	if (fd != STDIN_FILENO) {
	    if (close(fd) == -1) {
		syserr();
		uferr(fd);
		return ERROR;
	    }
	}

 /*
  * free dynamic arrays, then clear UIO control block
  */
	SAFE_FREE(p->name);
	SAFE_FREE(p->buf);

	bzero((void *) p, sizeof(*p));

	return (OK);
}
