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

#ifndef	_FPIO_H
#define	_FPIO_H

typedef struct {
	unsigned        flags;		/* bit flags			 */
	unsigned       *bflags;		/* per-band bit flags		 */
	int             nbands;		/* # bands			 */
	int             npixv;		/* requested # pixel vectors	 */
	pixel_t        *pixbuf;		/* -> base of pixel I/O buffer	 */
	fpixel_t      **map;		/* -> fpixel[band][pixel] map	 */
	int            *maplen;		/* -> map lengths		 */
	fpixel_t       *fmin;		/* -> min legal fpixel values	 */
	fpixel_t       *fmax;		/* -> max legal fpixel values	 */
	double        **lininv;		/* -> linear inverse[band][coef] */
} FPIO_T;

/* bits in "flags" */
#define	FPIO_NOIO	0		/* only initialize map stuff	 */
#define	FPIO_READ	BIT(0)		/* initialized for reading	 */
#define	FPIO_WRITE	BIT(1)		/* initialized for writing	 */

/* bits in "bflags" */
#define	FPIO_NOMAP	BIT(0)		/* convert by simple assignment	 */

/* ------------------------------------------------------------------------- */

/*
 *  These are the global variables internal to this directory.
 */

extern FPIO_T  **_fpiocb;

/* ------------------------------------------------------------------------- */

/*
 *  These routines are internal to this directory.
 */

extern FPIO_T    * _fpiocbinit(int fd, int flag, int npixv);
extern void	   _fpioinit(void);

#endif
