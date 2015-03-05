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
**	winhwrite -- write an IPW WINH header
**
** SYNOPSIS
**	#include "file.h"
**
**	int
**	winhwrite(
**		int             fd,	|* output file descriptor	 *|
**		WINH_T        **winhpp)	|* -> array of WINH pointers	 *|
**
** DESCRIPTION
**      winhwrite writes the array of WINH headers pointed to by winhpp
**	to file descriptor fd.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** HISTORY
**	5/3/93	Changed ftoa calls to dtoa.  All headers now write double
**		precision which should clean up some problems.  Dana Jacobsen.
*/

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "winh.h"
#include "_winh.h"

int
winhwrite(
	int             fd,		/* output file descriptor	 */
	WINH_T        **winhpp)		/* -> array of WINH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */

	nbands = hnbands(fd);

	assert(_winhcheck(winhpp, nbands));
 /*
  * loop through possible output bands
  */
	for (band = 0; band < nbands; ++band) {
		REG_1 WINH_T   *winhp;	/* -> current WINH		 */
		char            s[HREC_SIZ];	/* output value		 */

		winhp = winhpp[band];
		if (winhp == NULL) {
			continue;
		}
 /*
  * write preamble
  */
		if (hwprmb(fd, WINH_HNAME, band, WINH_VERSION) == ERROR) {
			return (ERROR);
		}
 /*
  * write fields
  */
		if (hputrec(fd, (char *) NULL, WINH_BLINE,
			    dtoa(s, winhp->bline))
		    == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, WINH_BSAMP,
			    dtoa(s, winhp->bsamp))
		    == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, WINH_DLINE,
			    dtoa(s, winhp->dline))
		    == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, WINH_DSAMP,
			    dtoa(s, winhp->dsamp))
		    == ERROR) {
			return (ERROR);
		}
	}

	return (OK);
}
