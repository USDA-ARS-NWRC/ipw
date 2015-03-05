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
**	imgcopy -- copy all image data
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	imgcopy(
**		int     i_fd,		|* input image file descriptor	 *|
**		int     o_fd)		|* output image file descriptor	 *|
**
** DESCRIPTION
**	imgcopy copies all image data from file descriptor i_fd to file
**	descriptor o_fd.
**
**	Image headers must be read from i_fd and written to o_fd before
**	calling imgcopy.
**
**	imgcopy is typically called by programs that add/delete/modify IPW
**	headers, but just pass image data through unaltered.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** ERRORS
**	can't calculate input image size
**		The total number of bytes in the input image wouldn't fit in a
**		signed long integer.
**
**	input image larger than header indicates
**		EOF was not reached after the appropriate number of bytes were
**		copied.
*/

#include "ipw.h"
#include "bih.h"

int
imgcopy(
	int             i_fd,		/* input image file descriptor	 */
	int             o_fd)		/* output image file descriptor	 */
{
	long            ncopy;		/* # bytes to copy		 */

 /*
  * ncopy <- # bytes of output image data
  */
	ncopy = imgsize(o_fd);
	if (ncopy == (long) ERROR) {
		uferr(i_fd);
		usrerr("can't calculate input image size");
		return (ERROR);
	}
 /*
  * copy ncopy bytes from i_fd to o_fd
  */
	if (ucopy(i_fd, o_fd, ncopy) != ncopy) {
		return (ERROR);
	}
 /*
  * we should be at EOF on i_fd
  */
	if (!ueof(i_fd)) {
		uferr(i_fd);
		usrerr("input image larger than header indicates");
		return (ERROR);
	}

	return (OK);
}
