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
**	ueof -- check for UIO end-of-file
**
** SYNOPSIS
**	#include "ipw.h"
**
**	bool_t
**	ueof(
**		int             fd)	|* file descriptor		 *|
**
** DESCRIPTION
**	ueof tests whether there any any data remaining to be read from file
**	descriptor fd, which must have been opened for reading by uropen.
**
**	ueof is typically called after an application program has finished
**	reading an image, to ensure that the header correctly represented the
**	size of the image.
**
** RESTRICTIONS
**
** RETURN VALUE
**	TRUE	End-of-file has been reached
**
**	FALSE	End-of-file has not been reached
**
** GLOBALS ACCESSED
**	_uiocb
*/

#include "ipw.h"
#include "_uio.h"

bool_t
ueof(
	int             fd)		/* file descriptor		 */
{
	UIO_T          *p;		/* -> UIO control block		 */

	ASSERT_OK_FD(fd);

	p = &_uiocb[fd];
	assert(p->flags & UIO_READ);

 /*
  * is EOF flag already set?
  */
	if (p->flags & UIO_EOF) {
		return (TRUE);
	}
 /*
  * else, is uio buffer (partially) full?
  */
	if (p->nbytes > 0) {
		return (FALSE);
	}
 /*
  * else, anything else to read?
  */
	return (_uread(p) == 0);
}
