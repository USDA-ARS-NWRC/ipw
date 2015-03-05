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
**	ugets -- read a line from a UNIX file descriptor
**
** SYNOPSIS
**	#include "ipw.h"
**
**	char *
**	ugets(
**		int      fd,		|* input file descriptor	 *|
**		char    *buf,		|* -> input buffer		 *|
**		int      nbytes)	|* #bytes to read		 *|
**
** DESCRIPTION
**	ugets reads bytes from fd into buf, until any of the following are
**	true:
**
**	-- nbytes-1 bytes have been read and transferred to buf
**	-- a newline character ('\n') has been read and transferred to buf
**	-- an end-of-file condition is encountered on fd
**
**	An EOS character ('\0') is placed after the last byte transferred to
**	buf (i.e., on return, buf will be a null-terminated C string).
**
**	ugets is analogous to the UNIX stdio routine fgets.
**
**	Use ugets for efficient input of ASCII text (e.g., image headers)
**	from IPW data files.
**
** RETURN VALUE
**	buf	The read was successful.
**
**	NULL	An error occurred or end-of-file was encountered.
**
** GLOBALS ACCESSED
**	_uiocb
*/

#include <strings.h>

#include "ipw.h"
#include "_uio.h"

char *
ugets(
	int             fd,		/* input file descriptor	 */
	char           *buf,		/* -> input buffer		 */
	int             nbytes)		/* #bytes to read		 */
{
	REG_3 int       ngot;		/* #bytes got this time around	 */
	REG_5 int       nwant;		/* #bytes still needed		 */
	REG_4 UIO_T    *p;		/* -> uio control block		 */
	REG_1 char     *q1;		/* scratch char ptr		 */
	REG_2 char     *q2;		/* scratch char ptr		 */
	char           *rtn;		/* -> returned string		 */

	ASSERT_OK_FD(fd);
	p = &_uiocb[fd];

	assert(p->flags & UIO_READ);

	ngot = 0;
	rtn = buf;

 /*
  * loop until we've obtained # bytes requested 
  */
	for (nwant = nbytes - 1; nwant > 0; nwant -= ngot) {
		if (p->nbytes <= 0) {
			if (_uread(p) <= 0) {
				break;
			}
		}

 /*
  * want to copy through first newline in buffer 
  */
		ngot = 0;
		q2 = p->eobuf;
		for (q1 = p->bufp; q1 != q2;) {
			++ngot;

			if (*q1++ == '\n') {
				break;
			}
		}

 /*
  * trim copy request to fit caller's buffer 
  */
		if (ngot > nwant) {
			ngot = nwant;
		}

 /*
  * if trimmed request still ends in a newline, then don't need to keep
  * reading 
  */
		if (p->bufp[ngot - 1] == '\n') {
			nwant = 0;
		}

		bcopy(p->bufp, buf, ngot);

 /*
  * update buffer pointers and UIO byte count 
  */
		buf += ngot;
		p->bufp += ngot;
		p->nbytes -= ngot;
	}

 /*
  * add trailing null 
  */
	*buf = EOS;

 /*
  * if request still unsatisfied, then there was a read error or end-of-file 
  */
	if (nwant > 0) {
		return (NULL);
	}

	return (rtn);
}
