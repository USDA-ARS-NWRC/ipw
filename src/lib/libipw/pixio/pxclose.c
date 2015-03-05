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
**	pxclose -- close pixel I/O file descriptor
**
** SYNOPSIS
**	#include "pixio.h"
**
**	int
**	pxclose(
**		int		fd)	|* file descriptor		*|
**
** DESCRIPTION
**	pxclose resets all pixel I/O information associated with file
**	descriptor fd, then calls uclose to close the associated file.
**
**	Call pxclose if you need to reclaim a file descriptor that has been
**	read|written by pvread|pvwrite.
**
** RESTRICTIONS
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**	_piocb
*/

#include "ipw.h"
#include "pixio.h"
#include "_pixio.h"
#include "../uio/_uio.h"	/* for _ugetlvl(), _usetlvl() */
#include "../bih/_bih.h"	/* for _bihfree() */

int
pxclose(
	int		fd)		/* file descriptor		*/
{
	PIXIO_T		*p;		/* -> pixel I/O control block	*/
	int		level;

	ASSERT_OK_FD(fd);

 /*
  * Is this the right call to be making?
  */
	level = _ugetlvl(fd);
	ASSERT_IO_LEVEL(FTYPE_PIXIO, level);
/*
	if (level != FTYPE_PIXIO) {
		if (level == FTYPE_UIO) {
			error("Calling pxclose instead of uclose");
		} else if (level == FTYPE_FPIO) {
			error("Calling pxclose instead of fpclose");
		} else {
			error("Calling uclose for unknown I/O level!");
		}
	}
*/

	p = _piocb[fd];

	/*
	 *  Note: p may be NULL if a "fpio" routine was called like "fpmax"
	 *        but no pixel I/O was done.  This is the case with the
	 *        command "lqmm".  This results in the level being set
	 *	  to FTYPE_FPIO, which causes "_uioexit" to invoke "fpclose".
	 *	  "fpclose" then invokes this routine, but since no "pixio"
	 *        routine was ever called, the file descriptor will have
	 *        no pio control block.
	 *	  J. Domingo, 4 Nov 1997
	 */
	if (p != NULL) {

		SAFE_FREE(p->rawbuf);
		SAFE_FREE((char *) p->pixsiz);
		SAFE_FREE((char *) p->bandsiz);
		SAFE_FREE((char *) p->pixmask);
		SAFE_FREE((char *) p->bandmask);

		SAFE_FREE((char *) p);

		_piocb[fd] = NULL;
	}

 	/*
   	 * now free the BIH information
  	 */
	_bihfree(fd);

 /*
  * We've done our work, so the correct level for this file is longer pixio.
  */
	_usetlvl(fd, -FTYPE_UIO);
	return (uclose(fd));
}
