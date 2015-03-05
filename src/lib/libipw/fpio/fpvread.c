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
**	fpvread -- read floating-point vectors
**
** SYNOPSIS
**	#include "fpio.h"
**
**	int
**	fpvread(
**		int             fd,	  |* input file descriptor	 *|
**		REG_2 fpixel_t *buf,	  |* -> floating-point buffer	 *|
**		REG_5 int       npixv)	  |* # pixel vectors to read	 *|
**
** DESCRIPTION
**	fpvread reads npixv floating-point vectors from file descriptor fd
**	into buf.
**
** RETURN VALUE
**	number of floating-point vectors read; else ERROR for failure
**
** GLOBALS ACCESSED
**	_fpiocb
*/

#include "ipw.h"
#include "_fpio.h"
#include "fpio.h"
#include "pixio.h"

int
fpvread(
	int             fd,		/* input file descriptor	 */
	REG_2 fpixel_t *buf,		/* -> floating-point buffer	 */
	REG_5 int       npixv)		/* # pixel vectors to read	 */
{
	REG_6 FPIO_T   *p;		/* -> f.p. pixel I/O ctrl block	 */
	REG_1 pixel_t  *pixp;		/* -> pixel buffer		 */
	int             rtn;		/* return value			 */

	p = _fpiocbinit(fd, FPIO_READ, npixv);
	if (p == NULL) {
		return (ERROR);
	}

 /*
  * read pixels
  */
	npixv = pvread(fd, p->pixbuf, p->npixv);
	if (npixv <= 0) {
		return (npixv);
	}

	rtn = npixv;

 /*
  * convert pixels -> floating point
  */
	pixp = p->pixbuf;

 /*
  * fast single-band case
  */
	if (p->nbands == 1) {
		REG_3 fpixel_t *map;	/* -> int->fp conversion map	 */

		map = p->map[0];

		do {
			*buf++ = map[*pixp++];
		} while (--npixv > 0);
	}

 /*
  * general multi-band case
  */
	else {
		do {
			REG_3 fpixel_t **map;	/* -> int->fp cvt. map	 */
			REG_4 int       nbands;	/* band counter		 */

			map = p->map;

			nbands = p->nbands;
			do {
				*buf++ = (*map++)[*pixp++];
			} while (--nbands > 0);
		} while (--npixv > 0);
	}

 /*
  * return # pixel vectors
  */
	return (rtn);
}
