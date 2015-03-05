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

#ifndef	_UIO_H
#define	_UIO_H

#include <sys/types.h>	/* for size_t */

/* per-fd I/O control block */
typedef struct {
	int             fd;		/* UNIX file descriptor		 */
	char           *name;		/* file name			 */
	unsigned        flags;		/* bit flags			 */
	char           *buf;		/* -> base of I/O buffer	 */
	size_t          bufsiz;		/* size (# bytes) of I/O buffer	 */
	char           *bufp;		/* -> current pos. in buffer	 */
	char           *eobuf;		/* -> 1 past last pos. in buffer */
	int             nbytes;		/* # bytes in buffer		 */
	int		level;		/* max I/O level		 */
} UIO_T;

#define	UIO_READ	BIT(0)		/* initialized for reading	 */
#define	UIO_WRITE	BIT(1)		/* initialized for writing	 */
#define	UIO_USED	BIT(2)		/* I/O done since u?open	 */
#define	UIO_EOF		BIT(3)		/* ? EOF from read()		 */
#define	UIO_CANSEEK	BIT(4)		/* ? OK to lseek()		 */

#define FTYPE_MIN	0
#define FTYPE_TXTIO	0
#define FTYPE_UIO	1
#define FTYPE_PIXIO	2
#define FTYPE_FPIO	3
#define FTYPE_MAX	4

#define TEXT_FTYPE(x)	( ((x) == FTYPE_UIO)     ? "uio"   : \
			   ((x) == FTYPE_PIXIO)  ? "pixio" : \
			    ((x) == FTYPE_FPIO)  ? "fpio"  : "unknown" \
			)
#define ASSERT_IO_LEVEL(routine,file)	if ((routine) != (file)) { \
			error("Program called %s close instead of %s close.", \
				TEXT_FTYPE(routine), TEXT_FTYPE(file)); \
					}

/* ------------------------------------------------------------------------- */

/*
 *  These are the global variables internal to this directory.
 */

extern	int    _ipwnumfds;
extern  int   *_ipwfds;
extern  UIO_T *_uiocb;

/*
 *  Note: _ipwmaxfd is declared in $IPW/h/_uio.h because of the macro
 *	  ASSERT_OK_FD.
 */

/* ------------------------------------------------------------------------- */

/*
 *  These routines are internal to this directory.
 */

extern int	_ugetlvel(int fd);
extern void	_uioexit(void);
extern void	_uioinit(void);
extern int	_uioopen(int fd, const char *name, int mode);
extern int	_uread(UIO_T *p);
extern void	_usetlvl(int fd, int level);
extern int	_uwrite(UIO_T *p);

#endif
