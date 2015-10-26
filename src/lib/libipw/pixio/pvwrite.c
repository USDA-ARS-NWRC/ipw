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
 **	pvwrite -- write pixel vectors
 **
 ** SYNOPSIS
 **	#include "pixio.h"
 **
 **	int
 **	pvwrite(
 **		int             fd,	  |* input file descriptor	 *|
 **		pixel_t        *buf,	  |* -> pixel buffer		 *|
 **		int             npixv)	  |* # pixel vectors to write	 *|
 **
 ** DESCRIPTION
 **	pvwrite writes npixv pixel vectors from buf into file descriptor fd.
 **
 **	pvwrite is the lowest level of IPW output that represents pixel values
 **	in a single format (unsigned integers).
 **
 ** RETURN VALUE
 **	number of pixel vectors written; else ERROR for failure
 **
 ** GLOBALS ACCESSED
 **	_piocb
 */

#include "ipw.h"
#include "pixio.h"
#include "_pixio.h"

int
pvwrite(
		int             fd,		/* input file descriptor	 */
		pixel_t        *buf,		/* -> pixel buffer		 */
		int             npixv)		/* # pixel vectors to write	 */
{
	REG_4 pixel_t  *bufp;		/* fast -> buf			 */
	REG_3 int      *ip;		/* fast -> pix{mask,size}	 */
	REG_6 int       n;		/* loop counter			 */
	REG_5 int       nbytes;		/* # bytes / raw output pixel	 */
	int             npixels;	/* total # pixels to write	 */
	PIXIO_T        *p;		/* -> pixel I/O control block	 */
	REG_1 char     *pixp;		/* -> byte within pixel_t	 */
	REG_2 char     *rawp;		/* -> raw output byte		 */

#if ! WORDS_BIGENDIAN
	int             xnbytes;	/* saved nbytes (littleendians)	 */

#endif

	p = _piocbinit(fd, PIXIO_WRITE, npixv);
	if (p == NULL) {
		return (ERROR);
	}

	npixels = npixv * p->nbands;

	/*
	 * mask off unused bits in buf
	 */
	ip = p->pixmask;
	bufp = buf;

	n = npixels;
	do {
		*bufp++ &= *ip++;
	} while (--n > 0);

	/*
	 * convert pixel_t -> raw pixels
	 */
	ip = p->pixsiz;
	rawp = p->rawbuf;
	/* NOSTRICT */
	pixp = (char *) buf;

	n = npixels;
	do {
		nbytes = *ip++;
#if WORDS_BIGENDIAN
		pixp += sizeof(pixel_t);
		pixp -= nbytes;
#else
		xnbytes = nbytes;
#endif
		do {
			*rawp++ = *pixp++;
		} while (--nbytes > 0);
#if ! WORDS_BIGENDIAN
		pixp += sizeof(pixel_t);
		pixp -= xnbytes;
#endif
	} while (--n > 0);
	/*
	 * write raw pixels
	 */
	if (uwrite(fd, p->rawbuf, p->nbytes) != p->nbytes) {
		return (ERROR);
	}

	/*
	 * return # pixel vectors
	 */
	return (npixv);
}
