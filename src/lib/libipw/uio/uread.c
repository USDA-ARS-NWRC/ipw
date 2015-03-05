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
**	uread -- UNIX read
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	uread(
**		int       fd,		|* input file descriptor	 *|
**		addr_t    buf,		|* -> input buffer		 *|
**		int       nbytes)	|* #bytes to read		 *|
**
** DESCRIPTION
**	uread is a reliable interface to the UNIX read function.  It is
**	guaranteed to return the number of bytes requested unless end-of-file
**	is reached, or an I/O error occurs.
**
**	Note that the semantics of UNIX pipes are such that a normal UNIX
**	read may return with less than the number of bytes requested, even
**	when the pipe is not empty; this was the principal motivation for
**	uread.
**
** RESTRICTIONS
**
** RETURN VALUE
**	>= 0	number of bytes read
**
**	ERROR	if failure
**
** GLOBALS ACCESSED
**	_uiocb
**
*/

#include <strings.h>

#include "ipw.h"
#include "_uio.h"

int
uread(
	int             fd,		/* input file descriptor	 */
	addr_t          buf,		/* -> input buffer		 */
	int             nbytes)		/* #bytes to read		 */
{
	REG_1 UIO_T    *p;		/* -> uio control block		 */
	REG_2 int       ngot;		/* #bytes got this time around	 */
	REG_3 int       nwant;		/* #bytes still needed		 */

	ASSERT_OK_FD(fd);
	p = &_uiocb[fd];

	assert(p->flags & UIO_READ);

	ngot = 0;

 /*
  * loop until we've obtained # bytes requested
  */
	for (nwant = nbytes; nwant > 0; nwant -= ngot) {
		if (p->nbytes <= 0) {
			if (_uread(p) <= 0) {
				break;
			}
		}

 /*
  * copy # bytes available or # bytes requested, whichever is smaller
  */
		ngot = MIN(p->nbytes, nwant);
		bcopy(p->bufp, buf, ngot);

 /*
  * update buffer pointers and UIO byte count
  */
		buf = (char *) buf + ngot;
		p->bufp += ngot;
		p->nbytes -= ngot;
	}

 /*
  * return actual # bytes obtained (or ERROR if most recent read failed)
  */
	if (p->nbytes < 0) {
		return (ERROR);
	}

	return (nbytes - nwant);
}
