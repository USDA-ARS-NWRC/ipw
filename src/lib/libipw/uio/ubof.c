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
**	ubof -- check for UIO beginning-of-file
**
** SYNOPSIS
**	#include "ipw.h"
**
**	bool_t
**	ubof(
**		int             fd)	|* file descriptor		 *|
**
** DESCRIPTION
**	ubof tests whether any I/O has been performed on file descriptor fd
**	since it was last u[rw]opened, i.e., are we at the Beginning Of
**	the File?
**
**	ubof exists mainly to allow the hrname to reset itself after a file
**	descriptor has been closed and reopened.  ubof will probably not need
**	to be called by IPW application programs.
**
** RETURN VALUE
**	TRUE	no I/O has been performed (i.e., at beginning of file)
**
**	FALSE	I/O has been performed (not at beginning of file)
**
** GLOBALS ACCESSED
**	_uiocb
*/

#include "ipw.h"
#include "_uio.h"

bool_t
ubof(
	int             fd)		/* file descriptor		 */
{
	ASSERT_OK_FD(fd);
	return ((_uiocb[fd].flags & UIO_USED) == 0);
}
