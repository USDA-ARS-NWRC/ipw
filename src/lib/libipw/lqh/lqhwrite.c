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
**	lqhwrite -- write an IPW LQH header
**
** SYNOPSIS
**	#include "lqh.h"
**
**	int
**	lqhwrite(
**		int      fd,		|* output file descriptor	 *|
**		LQH_T  **lqhpp)		|* -> array of LQH pointers	 *|
**
** DESCRIPTION
**      lqhwrite writes the array of LQH headers pointed to by lqhpp
**	to file descriptor fd.
**
**	lqhwrite is called by IPW application programs to write
**	LQH headers.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "_lqh.h"

int
lqhwrite(
	int             fd,		/* output file descriptor	 */
	LQH_T         **lqhpp)		/* -> array of LQH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */

	nbands = hnbands(fd);

	assert(_lqhcheck(lqhpp, nbands));

 /*
  * loop through possible output bands
  */
	for (band = 0; band < nbands; ++band) {
		int             i;	/* loop counter			 */
		REG_1 LQH_T    *lqhp;	/* -> current LQH		 */
		char            s[HREC_SIZ];	/* output value		 */

		lqhp = lqhpp[band];
		if (lqhp == NULL) {
			continue;
		}
 /*
  * write preamble
  */
		if (hwprmb(fd, LQH_HNAME, band, LQH_VERSION) == ERROR) {
			return (ERROR);
		}

 /*
  * write fields
  */
		if (lqhp->units != NULL) {
			if (hputrec(fd, (char *) NULL, LQH_UNITS, lqhp->units)
			    == ERROR) {
				return (ERROR);
			}
		}

		if (lqhp->interp != NULL) {
			if (hputrec(fd, (char *) NULL, LQH_INTERP,
				    lqhp->interp)
			    == ERROR) {
				return (ERROR);
			}
		}

		for (i = 0; i < lqhp->nbkpts; ++i) {
			(void) sprintf(s, "%d %.10g",
				       lqhp->bkpt[i],
				       lqhp->map[lqhp->bkpt[i]]);

			if (hputrec(fd, (char *) NULL, LQH_MAP, s)
			    == ERROR) {
				return (ERROR);
			}
		}
	}

	_lqh[fd] = lqhpp;
	return (OK);
}
