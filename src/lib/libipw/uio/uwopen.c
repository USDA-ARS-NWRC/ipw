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
**	uwopen -- open UNIX file for writing
**
** SYNOPSIS
**	int
**	uwopen(
**		const char     *name)	|* file name			 *|
**
** DESCRIPTION
**	uwopen opens the file name for writing.  The file is truncated if it
**	already exists.
**
**	Use uwopen wherever you would normally use the UNIX system call
**	creat() to open a file for writing.
**
** RESTRICTIONS
**
** RETURN VALUE
**	>= 0	A writable UNIX file descriptor for successful open.
**
**	ERROR	Error occurred.
**
** GLOBALS ACCESSED
*/

#include <fcntl.h>	/* for creat() */
#include <sys/stat.h>	/* for S_I* bit flags */

#include "ipw.h"
#include "_uio.h"

#define CREAT_MODE  (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
    /*
     *  The access permission for the creat() call.  It is rw-rw-rw- 
     *	(read-write for user, group and others).  This is the default;
     *  it is masked with the user's umask value which may turn off
     *	some of the bits.
     */

int
uwopen(
	const char     *name)		/* file name			 */
{
	int             fd;		/* file descriptor		 */

	assert(name != NULL);

	fd = creat(name, CREAT_MODE);
	if (fd == -1) {
		syserr();
		return (ERROR);
	}

	if (_uioopen(fd, name, UIO_WRITE) == ERROR) {
		return (ERROR);
	}

	return (fd);
}
