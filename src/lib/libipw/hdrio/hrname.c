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
**	hrname -- get name of current input image header
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	char *
**	hrname(
**		int     fd)		|* input file descriptor	 *|
**
** DESCRIPTION
**	hrname returns the name of the image header currently being read from
**	file descriptor fd.
**
**	Call hrname to determine what kind of header is waiting to be read;
**	then, call either hrskip, hpass, or a header-specific ingest function.
**
** RESTRICTIONS
**	The return value points to static data which may be overwritten by
**	subsequent calls.
**
** RETURN VALUE
**	A pointer to the EOS-terminated header name is returned.  NULL is
**	returned on EOF or error.  The string symbolically defined as BOIMAGE
**	is returned if there are no more headers to be read.
**
** GLOBALS ACCESSED
**	_hdriocb
**
*/

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

char *
hrname(
	int             fd)		/* input file descriptor	 */
{
	if (ubof(fd) || _hdriocb[fd].name == NULL) {
 /*
  * A NULL name is the initial state of the system before any I/O has
  * occurred.  We will therefore try to read a single header record.  If it's
  * not a preamble record, then we assume that the input is not a valid IPW
  * image.
  */
		int             err;	/* returned from _hrrec()	 */

		char            buf[HREC_MAX + 1];	/* record buffer */

		err = _hrrec(fd, buf, sizeof(buf));
		if (err != HGOT_PRMB) {
			uferr(fd);
			usrerr("invalid IPW image (no header)");
			return (NULL);
		}
	}

	return (_hdriocb[fd].name);
}
