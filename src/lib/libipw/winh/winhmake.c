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
**	winhmake -- make an IPW WINH header
**
** SYNOPSIS
**	#include "winh.h"
**
**	WINH_T *
**	winhmake(
**		double    bline,	|* begin line #			 *|
**		double    bsamp,	|* begin sample #		 *|
**		double    dline,	|* line increment		 *|
**		double    dsamp)	|* sample increment		 *|
**
** DESCRIPTION
**	winhmake allocates a WIN header.  The header is initialized with the
**	remaining arguments.
**
** RETURN VALUE
**	pointer to new WIN header; NULL if error
*/

#include "ipw.h"
#include "hdrio.h"
#include "winh.h"

WINH_T *
winhmake(
	double          bline,		/* begin line #			 */
	double          bsamp,		/* begin sample #		 */
	double          dline,		/* line increment		 */
	double          dsamp)		/* sample increment		 */
{
	WINH_T         *winhp;		/* -> WIN header		 */

	assert(dline != 0.0);
	assert(dsamp != 0.0);
 /*
  * allocate header
  */
 /* NOSTRICT */
	winhp = (WINH_T *) hdralloc(1, sizeof(WINH_T), ERROR, WINH_HNAME);
	if (winhp == NULL) {
		return (NULL);
	}
 /*
  * initialize
  */
	winhp->bline = bline;
	winhp->bsamp = bsamp;
	winhp->dline = dline;
	winhp->dsamp = dsamp;

	return (winhp);
}
