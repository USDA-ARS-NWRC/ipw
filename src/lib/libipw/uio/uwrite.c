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
**	uwrite -- UNIX write
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	uwrite(
**		int             fd,	   |* output file descriptor	 *|
**		const addr_t    buf,	   |* buffer to write from	 *|
**		int             nbytes)	   |* number of bytes to write	 *|
**
** DESCRIPTION
**	uwrite is a reliable interface to the UNIX write function.  The
**	various types of write errors are detected and folded into a single
**	error return.
**
**	It is considered an error for UNIX write to return any number other
**	than the number of bytes that were requested to be written.
**
**	Use uwrite wherever you would normally use the UNIX system call
**	write.
**
** RETURN VALUE
**	>= 0	number of bytes written
**
**	ERROR	for failure
**
** GLOBALS ACCESSED
**	_uiocb
**
** DIAGNOSTICS
**	"incomplete write"
**		some number of bytes less than the number requested were
**		written.  This is not necessarily due to a hard I/O error.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

#include <strings.h>

#include "ipw.h"
#include "_uio.h"

int
uwrite(
	int             fd,		/* output file descriptor	 */
	const addr_t    buf,		/* buffer to write from		 */
	int             nbytes)		/* number of bytes to write	 */
{
	REG_2 int      	ncopy;		/* # bytes to copy		 */
	REG_3 int      	nleft;		/* # bytes not yet copied	 */
	REG_1 UIO_T    *p;		/* -> UIO control block		 */
	char           *bufp;

	ASSERT_OK_FD(fd);
	p = &_uiocb[fd];

	assert(p->flags & UIO_WRITE);
	bufp = buf;
	ncopy = 0;

 /*
  * loop until we've written # bytes requested 
  */
	for (nleft = nbytes; nleft > 0; nleft -= ncopy) {
		if (p->nbytes >= p->bufsiz) {
			if (_uwrite(p) == ERROR) {

 /*
  * return immediately if write error 
  */
				return (ERROR);
			}
		}

 /*
  * copy # bytes available or # bytes requested, whichever is smaller 
  */
		ncopy = p->bufsiz - p->nbytes;
		if (ncopy > nleft) {
			ncopy = nleft;
		}

		bcopy(bufp, p->bufp, ncopy);

 /*
  * update buffer pointers and UIO byte count 
  */
		bufp += ncopy;
		p->bufp += ncopy;
		p->nbytes += ncopy;
	}

 /*
  * return actual # bytes written 
  */
	return (nbytes - nleft);
}
