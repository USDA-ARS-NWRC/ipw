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
**	_uioopen -- initialize newly opened IPW file
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int
**	_uioopen(
**		int       fd,		|* file descriptor		 *|
**		char     *name,		|* file name			 *|
**		int       mode)		|* UIO_{READ,WRITE}		 *|
**
** DESCRIPTION
**	_uioopen initializes the internal data structures associated with
**	the newly opened file descriptor fd.  mode is either UIO_READ or
**	UIO_WRITE.  name is the UNIX file name associated with fd.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS MODIFIED
**	_ipwfds
**	_ipwnumfds
**	_uiocb
**
*/

#include <unistd.h>	/* for lseek()  */
#include <errno.h>	/* for errno    */
#include <string.h>	/* for strdup() */

#include "ipw.h"
#include "_uio.h"

int
_uioopen(
	int             fd,		/* file descriptor		 */
	const char           *name,		/* file name			 */
	int             mode)		/* UIO_{READ,WRITE}		 */
{
	UIO_T          *p;		/* -> UIO control block		 */

 /*
  * allow read-only or write-only, but not read-write access 
  */
	assert(mode == UIO_READ || mode == UIO_WRITE);

	ASSERT_OK_FD(fd);
	p = &_uiocb[fd];

 /*
  * barf if already initialized 
  */
	assert(p->flags == 0);

 /*
  * add file desc to list of open files
  */
	_ipwfds[_ipwnumfds++] = fd;

 /*
  * set I/O buffer size
  */
	p->bufsiz = OPT_BUF;

 /*
  * allocate I/O buffer
  */
	p->buf = ecalloc((int) p->bufsiz, sizeof(char));
	if (p->buf == NULL) {
		uferr(fd);
		usrerr("can't allocate uio buffer");
		return (ERROR);
	}

 /*
  * remaining initializations 
  */
	p->eobuf = p->buf + p->bufsiz;
	p->bufp = p->buf;
	p->nbytes = 0;

	p->level = FTYPE_UIO;

	p->fd = fd;
	p->flags = mode;

	if (lseek(fd, (off_t) 0, SEEK_CUR) != (off_t) -1) {
		p->flags |= UIO_CANSEEK;
	} else {
		/*
		 *  Clear error flag.
		 */
		errno = 0;
	}

	p->name = strdup(name);
	if (p->name == NULL) {
		return (ERROR);
	}

	return (OK);
}
