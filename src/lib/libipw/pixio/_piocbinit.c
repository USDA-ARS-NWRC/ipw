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
**	_piocbinit -- initialize pixel I/O on file descriptor
**
** SYNOPSIS
**	#include "_pixio.h"
**
**	PIXIO_T *
**	_piocbinit(
**		int     fd,		|* file descriptor		 *|
**		int     flag,		|* I/O flag(s)			 *|
**		int     npixv)		|* # pixel vectors to read/write *|
**
** DESCRIPTION
**	_pioinit initializes file descriptor fd for pixel-oriented I/O. flag
**	is either PIXIO_READ or PIXIO_WRITE; simultanneous read/write access
**	is NOT allowed.  npixv is the number of raw image pixel vectors (1
**	pixel vector == all bands at a given line,sample) to be internally
**	buffered for pvread or pvwrite.
**
** RETURN VALUE
**	pointer to pixel I/O control block; else NULL if error.
**
** GLOBALS ACCESSED
**	_piocb
**
** ERRORS
**	{#bytes}-byte pixels not supported
**	can't allocate internal pixel mask array
**	band {band#}: {#bits}-bit pixels too large
**	can't allocate internal pixel buffer
*/

#include "ipw.h"
#include "pixio.h"
#include "_pixio.h"
#include "bih.h"		/* for hostorder(), hbyteorder() */
#include "../uio/_uio.h"	/* for _usetlvl() */


static int       newbufs(PIXIO_T *p);
static PIXIO_T * newcb(int fd, int flag);


PIXIO_T *
_piocbinit(
	int             fd,		/* file descriptor		 */
	int             flag,		/* I/O flag(s)			 */
	int             npixv)		/* # pixel vectors to read/write */
{

	REG_1 PIXIO_T  *p;		/* -> pixel I/O control block	 */

	ASSERT_OK_FD(fd);
	assert(npixv > 0);

	p = _piocb[fd];
 /*
  * if no control block then allocate a new one
  */
	if (p == NULL) {
		p = newcb(fd, flag);
		if (p == NULL) {
			uferr(fd);
			return (NULL);
		}
	}
 /*
  * barf if control block has wrong R/W mode (this looks redundant if we just
  * allocated a new control block, but this is also where we check that "flag"
  * is valid)
  *
  * NB: you could conceivably be fooled by a "stale" control block here that
  * just happened to have the correct R/W mode.  Moral: call pxclose() before
  * you recycle file descriptors!
  */
	if (flag == PIXIO_READ) {
		if (p->flags & PIXIO_WRITE) {
			uferr(fd);
			bug("attempt to read from write-only file");
		}
	}
	else if (flag == PIXIO_WRITE) {
		if (p->flags & PIXIO_READ) {
			uferr(fd);
			bug("attempt to write to read-only file");
		}
	}
	else if (flag != 0) {
		uferr(fd);
		bug("bad pixio read/write mode");
	}
 /*
  * ensure raw buffer and associated data structures are large enough for #
  * pixels requested
  */
	if (npixv != p->npixv) {
		p->npixv = npixv;
		p->npixels = p->npixv * p->nbands;
		p->nbytes = p->npixv * p->pixvsiz;

		if (p->nbytes > p->rawsiz) {
			if (newbufs(p) == ERROR) {
				uferr(fd);
				return (NULL);
			}
		}
	}
 /*
  * return -> control block
  */
	return (p);
}

/* ------------------------------------------------------------------------ */

static PIXIO_T *
newcb(
	int             fd,		/* file descriptor		 */
	int             flag)		/* I/O flag(s)			 */
{
	REG_2 int       band;		/* loop counter			 */
	REG_1 PIXIO_T  *p;		/* -> pixel I/O control block	 */

 /*
  * allocate new control block
  */
 /* NOSTRICT */
	p = (PIXIO_T *) ecalloc(1, sizeof(PIXIO_T));
	if (p == NULL) {
		return (NULL);
	}
 /*
  * scalar initializations
  */
	p->flags = flag;
	p->nbands = hnbands(fd);

 /*
  * check for wrong byteorder
  */
	if ( strdiff( hostorder(), hbyteorder(fd) ) ) {
		p->flags |= PIXIO_SWAP;
	}
 /*
  * array of per-band pixel sizes
  */
 /* NOSTRICT */
	p->bandsiz = (int *) ecalloc(p->nbands, sizeof(int));
	if (p->bandsiz == NULL) {
		usrerr("can't allocate internal pixel size array");
		return (NULL);
	}
 /*
  * array of bit masks for pixel_t <-> raw conversion
  */
 /* NOSTRICT */
	p->bandmask = (int *) ecalloc(p->nbands, sizeof(int));
	if (p->bandmask == NULL) {
		usrerr("can't allocate internal pixel mask array");
		return (NULL);
	}
 /*
  * per-band initializations
  */
	for (band = 0; band < p->nbands; ++band) {
		int             nbits;	/* # bits / pixel		 */

		p->bandsiz[band] = hnbytes(fd, band);
		if (p->bandsiz[band] > sizeof(pixel_t)) {
			usrerr("%d-byte pixels not supported",
			       p->bandsiz[band]);
			return (NULL);
		}

		p->pixvsiz += p->bandsiz[band];

		nbits = hnbits(fd, band);
		if ( (nbits < 1) || (nbits > (sizeof(pixel_t) * CHAR_BIT)) ) {
			usrerr("band %d: can't handle %d-bit pixels",
			       band, nbits);
			return (NULL);
		}

		p->bandmask[band] = MASK(nbits);
	}
 /*
  * we've succeeded -- set our I/O level in the UIO block.
  */
	_usetlvl(fd, FTYPE_PIXIO);

 /*
  * save control block
  */
	_piocb[fd] = p;
	return (p);
}

/* ------------------------------------------------------------------------ */

static int
newbufs(
	REG_1 PIXIO_T  *p)		/* -> pixel I/O control block	 */
{
	REG_2 int       band;		/* band counter			 */
	REG_3 int       pixel;		/* pixel counter		 */

 /*
  * allocate new {raw data,pixel size,pixel mask} arrays
  */
	SAFE_FREE((char *) p->rawbuf);
 /* NOSTRICT */
	SAFE_FREE((char *) p->pixsiz);
 /* NOSTRICT */
	SAFE_FREE((char *) p->pixmask);

	p->rawbuf = ecalloc(p->nbytes, sizeof(char));
 /* NOSTRICT */
	p->pixsiz = (int *) ecalloc(p->npixels, sizeof(int));
 /* NOSTRICT */
	p->pixmask = (int *) ecalloc(p->npixels, sizeof(int));

	if ((p->rawbuf == NULL) || (p->pixsiz == NULL) || (p->pixmask == NULL)){
		usrerr("can't allocate pixel I/O workspace");
		return (ERROR);
	}
 /*
  * initialize pixel {size,mask} arrays
  */
	band = 0;
	for (pixel = 0; pixel < p->npixels; ++pixel) {
		p->pixsiz[pixel] = p->bandsiz[band];
		p->pixmask[pixel] = p->bandmask[band];

		++band;
		if (band >= p->nbands) {
			band = 0;
		}
	}

	return (OK);
}
