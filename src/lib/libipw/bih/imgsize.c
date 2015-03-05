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
**	imgsize -- compute total # bytes in IPW image
**
** SYNOPSIS
**	#include "bih.h"
**
**	long
**	imgsize(
**		int             fd)	|* image file descriptor	 *|
**
** DESCRIPTION
**	imgsize computes the total number of image data (pixel) bytes in the
**	image accessed via file descriptor fd.
**
**	The value returned by imgsize does NOT include the bytes occupied by
**	header data (e.g., for an image with 512 lines, 512 samples, 1 band,
**	and 1 byte per pixel, the value returned by imgsize will be 512*512).
**
**	imgsize is typically called by application programs that modify image
**	headers and then copy all image data by calling ucopy.
**
** RETURN VALUE
**	total # bytes in image
*/

#include "ipw.h"
#include "_bih.h"

long
imgsize(
	int             fd)		/* image file descriptor	 */
{
	BIH_T         **bihpp;		/* -> BIH array			 */
	long            nbytes;		/* # bytes in image		 */

	bihpp = _bih[fd];
	assert(bihpp != NULL);

	nbytes = sampsize(fd);
	nbytes *= bih_nsamps(bihpp[0]);
	nbytes *= bih_nlines(bihpp[0]);

	return (nbytes);
}

/* ----------------------------------------------------------------------- */

#ifdef TEST

main(argc, argv)
	int             argc;
	char          **argv;
{
	int             fdi;

	ipwenter(argc, argv, (OPTION_T **) NULL, "test imgsize() function");

	fdi = ustdin();
	no_tty(fdi);

	if (bihread(fdi) == NULL) {
		error("can't read BIH");
	}

	(void) printf("%ld\n", imgsize(fdi));
}

#endif
