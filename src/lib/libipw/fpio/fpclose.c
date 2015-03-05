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
**	fpclose -- close floating-point I/O file descriptor
**
** SYNOPSIS
**	#include "fpio.h"
**
**	int
**	fpclose(
**		int fd)		|* file descriptor		*|
**
** DESCRIPTION
**	fpclose resets all floating-point I/O information associated with file
**	descriptor fd, then calls pxclose to close the associated file.
**
**	Call fpclose if you need to reclaim a file descriptor that has been
**	read|written by fpvread|fpvwrite.
**
**	Do not attempt any FP operations (including LQH access) to the file
**	descriptor after this function is called.
**
** RESTRICTIONS
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**	_fpiocb
*/

#include "ipw.h"
#include "_fpio.h"
#include "fpio.h"
#include "pixio.h"
#include "../uio/_uio.h"	/* for _ugetlvl(), _usetlvl() */
#include "lqh.h"
#include "../lqh/_lqh.h"	/* for _lqhfree() */

int
fpclose(
	int             fd)		/* file descriptor		 */
{
	int		band;		/* current band #		*/
	FPIO_T         *p;		/* -> floating-pt I/O ctrl block */
	int		level;

	ASSERT_OK_FD(fd);

 /*
  * Is this the right call to be making?
  */
	level = _ugetlvl(fd);
	if (level != FTYPE_FPIO) {
		if (level == FTYPE_UIO) {
			error("Calling fpclose instead of uclose");
		} else if (level == FTYPE_PIXIO) {
			error("Calling fpclose instead of pxclose");
		} else {
			error("Calling uclose for unknown I/O level!");
		}
	}
	
	p = _fpiocb[fd];
	assert(p != NULL);

	for (band = 0; band < p->nbands; band++) {
 /* NOSTRICT */
		SAFE_FREE(p->map[band]);
 /* NOSTRICT */
		SAFE_FREE(p->lininv[band]);
	}

 /* NOSTRICT */
	SAFE_FREE(p->bflags);
 /* NOSTRICT */
	SAFE_FREE(p->pixbuf);
 /* NOSTRICT */
	SAFE_FREE(p->map);
 /* NOSTRICT */
	SAFE_FREE(p->maplen);
 /* NOSTRICT */
	SAFE_FREE(p->fmin);
 /* NOSTRICT */
	SAFE_FREE(p->fmax);
 /* NOSTRICT */
	SAFE_FREE(p->lininv);

 /*
  * now free the LQH information
  */
	_lqhfree(fd);

 /* NOSTRICT */
	SAFE_FREE(p);

	_fpiocb[fd] = NULL;

 /*
  * We've done our work, so the correct level for this file is longer fpio.
  */
	_usetlvl(fd, -FTYPE_PIXIO);
	return (pxclose(fd));
}
