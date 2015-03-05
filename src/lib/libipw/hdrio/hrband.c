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
**	hrband -- get current header band number
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	int
**	hrband(
**		int      fd)		|* input file descriptor	 *|
**
** DESCRIPTION
**	hrband returns the band number of the image header currently being
**	read from file descriptor fd, or NO_BAND if the header applies to all
**	bands in the image.
**
**	Call hrband after you call hrname, to see which band (if any) the
**	header belongs to.
**
** RESTRICTIONS
**
** RETURN VALUE
**	Current image header band number, or NO_BAND.
**
** GLOBALS ACCESSED
**	_hdriocb[fd]
**
** FUTURE DIRECTIONS
**
** BUGS
**	The NO_BAND value is necessarily negative, since any positive integer
**	is a valid band number.  It is therefore easy to mistake NO_BAND for
**	an error return.
**
**	Hrband returns NO_BAND even if there is no current image header
*/

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

int
hrband(
	int             fd)		/* input file descriptor	 */
{
	ASSERT_OK_FD(fd);
 /*
  * if there's no current dataset-header, then don't look at the band number
  * (it is initialized to 0, which appears to be a valid band #)
  */
	if (_hdriocb[fd].name == NULL) {
		return (NO_BAND);
	}

	return (_hdriocb[fd].band);
}
