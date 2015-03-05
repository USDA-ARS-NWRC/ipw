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
**	_fpiocbinit -- initialize floating-point I/O on file descriptor
**
** SYNOPSIS
**	#include "_fpio.h"
**
**	FPIO_T *
**	_fpiocbinit(
**		int     fd,		|* file descriptor		 *|
**		int     flag,		|* I/O flag(s)			 *|
**		int     npixv)		|* # pixel vectors to read/write *|
**
** DESCRIPTION
**	_fpiocbinit() initializes file descriptor fd for floating-point I/O.
**
**	If flag is FPIO_READ or FPIO_WRITE then npixv is the number of image
**	pixel vectors (1 pixel vector == all bands at a given line,sample) to
**	be internally buffered for fpvread or fpvwrite.  Simultanneous
**	read/write access is NOT allowed.
**
**	If flag is 0, then npixv is ignored.  This form of _fpiocbinit() is
**	used by the routines that provide access to the fpio data structures,
**	to guarantee that they are properly initialized even if no I/O has yet
**	been performed.
**
** RETURN VALUE
**	pointer to floating-point I/O control block; else NULL if error.
**
** GLOBALS ACCESSED
**	_fpiocb
*/

#include "ipw.h"
#include "_fpio.h"
#include "fpio.h"
#include "lqh.h"
#include "../lqh/_lqh.h"	/* for _lqhinterp() */
#include "../uio/_uio.h"	/* for _usetlvl() */

static FPIO_T * newcb(int fd, int flag);
static int      newbufs(FPIO_T *p);
static int      lqhinit(addr_t hpp, int band, FPIO_T *p);
static int      dflt_init(addr_t hpp, int band, FPIO_T *p);


FPIO_T *
_fpiocbinit(
	int             fd,		/* file descriptor		 */
	int             flag,		/* I/O flag(s)			 */
	int             npixv)		/* # pixel vectors to read/write */
{
	REG_1 FPIO_T   *p;		/* -> pixel I/O control block	 */

	ASSERT_OK_FD(fd);

	p = _fpiocb[fd];

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
  * just happened to have the correct R/W mode.  Moral: call fpclose() before
  * you recycle file descriptors!
  */
	if (flag == FPIO_READ) {
		if (p->flags & FPIO_WRITE) {
			uferr(fd);
			bug("attempt to read from write-only file");
		}
	}
	else if (flag == FPIO_WRITE) {
		if (p->flags & FPIO_READ) {
			uferr(fd);
			bug("attempt to write to read-only file");
		}
	}
	else if (flag != 0) {
		uferr(fd);
		bug("bad fpio read/write mode");
	}

 /*
  * ensure pixel buffer and associated data structures are large enough for #
  * pixels requested
  */
	if (npixv > p->npixv) {
		p->npixv = npixv;

		if (newbufs(p) == ERROR) {
			uferr(fd);
			return (NULL);
		}
	}

 /*
  * return -> control block
  */
	return (p);
}

/* ------------------------------------------------------------------------ */

static FPIO_T *
newcb(
	int             fd,		/* file descriptor		 */
	int             flag)		/* I/O flag(s)			 */
{
	int             band;		/* loop counter			 */
	int	(*hinit)(addr_t hhpp, int hband, FPIO_T *hp);
	addr_t          hpp;		/* -> initializer header	 */
	FPIO_T         *p;		/* -> pixel I/O control block	 */

 /*
  * allocate new control block
  */
 /* NOSTRICT */
	p = (FPIO_T *) ecalloc(1, sizeof(FPIO_T));
	if (p == NULL) {
		usrerr("can't allocate floating-point I/O control block");
		return (NULL);
	}

 /*
  * initialize scalars
  */
	p->flags = flag;
	p->nbands = hnbands(fd);

 /*
  * allocate arrays
  */
 /* NOSTRICT */
	p->bflags = (unsigned *) ecalloc(p->nbands, sizeof(unsigned));
 /* NOSTRICT */
	p->map = (fpixel_t **) ecalloc(p->nbands, sizeof(fpixel_t *));
 /* NOSTRICT */
	p->maplen = (int *) ecalloc(p->nbands, sizeof(int));
 /* NOSTRICT */
	p->fmin = (fpixel_t *) ecalloc(p->nbands, sizeof(fpixel_t));
 /* NOSTRICT */
	p->fmax = (fpixel_t *) ecalloc(p->nbands, sizeof(fpixel_t));
 /* NOSTRICT */
	p->lininv = (double **) ecalloc(p->nbands, sizeof(double *));

	if (p->bflags == NULL ||
	    p->map == NULL ||
	    p->maplen == NULL ||
	    p->lininv == NULL) {
		usrerr("can't pixel->floating-point conversion arrays");
		return (NULL);
	}

 /*
  * select initializer header
  */
 /* NOSTRICT */
	if ((hpp = (addr_t) lqh(fd)) != NULL) {
		hinit = lqhinit;
	}

 /*
  * NB: add support for alternative headers here
  */
	else {
		hinit = dflt_init;
	}

 /*
  * initialize arrays
  */
	for (band = 0; band < p->nbands; ++band) {
		p->maplen[band] = ipow2(hnbits(fd, band));
		if (p->maplen[band] <= 0) {
			usrerr("can't handle %d-bit pixels", hnbits(fd, band));
			return (NULL);
		}

		if ((*hinit) (hpp, band, p) == ERROR) {
			return (NULL);
		}

		p->fmin[band] = p->map[band][0];
		p->fmax[band] = p->map[band][p->maplen[band] - 1];

		if (p->fmin[band] > p->fmax[band]) {
			fpixel_t        tmp;

			tmp = p->fmin[band];
			p->fmin[band] = p->fmax[band];
			p->fmax[band] = tmp;
		}
	}

 /*
  * we've succeeded -- set our I/O level in the UIO block.
  */
	_usetlvl(fd, FTYPE_FPIO);

 /*
  * save control block
  */
	_fpiocb[fd] = p;
	return (p);
}

/* ------------------------------------------------------------------------ */

static int
newbufs(
	FPIO_T         *p)		/* -> pixel I/O control block	 */
{
 /*
  * allocate new {raw data,pixel size,pixel mask} arrays
  */
 /* NOSTRICT */
	SAFE_FREE((char *) p->pixbuf);

 /* NOSTRICT */
	p->pixbuf = (pixel_t *) ecalloc(p->npixv * p->nbands, sizeof(pixel_t));
	if (p->pixbuf == NULL) {
		usrerr("can't allocate floating-point I/O pixel buffer");
		return (ERROR);
	}

	return (OK);
}

/* ------------------------------------------------------------------------ */

static int
lqhinit(
	addr_t          hpp,		/* generic -> header array	 */
	int             band,		/* image band #			 */
	FPIO_T         *p)		/* -> pixel I/O control block	 */
{
	LQH_T         **lqhpp;

 /* NOSTRICT */
	lqhpp = (LQH_T **) hpp;

 /*
  * make sure THIS PARTICULAR BAND has an LQH
  */
	if (lqhpp[band] == NULL) {
		return (dflt_init(hpp, band, p));
	}

	if (_lqhinterp(lqhpp, p->nbands) == ERROR) {
		return (ERROR);
	}

	assert(p->maplen[band] == lqh_maplen(lqhpp[band]));

	p->map[band] = lqh_map(lqhpp[band]);
	p->lininv[band] = lqh_lininv(lqhpp[band]);

	return (OK);
}

/* ------------------------------------------------------------------------ */

/*
 * NB: add alternative initializer functions here
 */

/*
 * default initialization function
 */

/* ARGSUSED */
static int
dflt_init(
	addr_t          hpp,		/* generic -> header array	 */
	int             band,		/* image band #			 */
	FPIO_T         *p)		/* -> pixel I/O control block	 */
{
	fpixel_t       *map;		/* -> fpixel[pixel] map		 */
	int             pixdomain;	/* # possible pixel values	 */
	int             pixel;		/* loop counter			 */

	pixdomain = p->maplen[band];
 /* NOSTRICT */
	map = (fpixel_t *) ecalloc(pixdomain, sizeof(fpixel_t));
 /* NOSTRICT */
	p->lininv[band] = (double *) ecalloc(2, sizeof(double));

	if (map == NULL || p->lininv == NULL) {
		usrerr("can't allocate default fpixel[pixel] map");
		return (ERROR);
	}

	p->map[band] = map;

 /*
  * load a fpixel == pixel map
  */
	for (pixel = 0; pixel < pixdomain; ++pixel) {
		*map++ = pixel;
	}

 /*
  * set NOMAP flag so inverse will be done by simple assignment
  */
	p->bflags[band] |= FPIO_NOMAP;

	return (OK);
}
