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

/* LINTLIBRARY */

#include "ipw.h"
#include "hdrio.h"
#include "_bih.h"
#include "../main/_ipwmain.h"	/* for _cmdline() */

/*
** NAME
**	bihwrite -- write an IPW BIH header
**
** SYNOPSIS
**	#include "bih.h"
**
**	int
**	bihwrite(
**		int      fd,		|* output file descriptor	 *|
**		BIH_T  **bihpp)		|* -> array of BIH pointers	 *|
**
** DESCRIPTION
**      bihwrite writes the array of BIH headers pointed to by bihpp
**	to file descriptor fd.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

int
bihwrite(
	int             fd,		/* output file descriptor	 */
	BIH_T         **bihpp)		/* -> array of BIH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	REG_1 BIH_T    *bihp;		/* -> current BIH		 */
	int             nbands;		/* # bands in output image	 */
	bool_t          wrote_hdr;	/* ? at least 1 header written	 */

	char            value[HREC_SIZ];/* output value			 */

	assert(_bihcheck(bihpp));

 /*
  * write per-image component
  */
	bihp = bihpp[0];

	if (hwprmb(fd, BIHI_HNAME, NO_BAND, BIH_VERSION) == ERROR) {
		return (ERROR);
	}

	if (hputrec(fd, (char *) NULL, BIH_BYTEORDER, bihp->img->byteorder)
	    == ERROR) {
		return (ERROR);
	}

	if (hputrec(fd, (char *) NULL, BIH_NLINES,
		    itoa(value, bihp->img->nlines)) == ERROR) {
		return (ERROR);
	}

	if (hputrec(fd, (char *) NULL, BIH_NSAMPS,
		    itoa(value, bihp->img->nsamps)) == ERROR) {
		return (ERROR);
	}

	if (hputrec(fd, (char *) NULL, BIH_NBANDS,
		    itoa(value, bihp->img->nbands)) == ERROR) {
		return (ERROR);
	}
 /*
  * write per-band component(s)
  */
	wrote_hdr = FALSE;

	nbands = bihp->img->nbands;

	for (band = 0; band < nbands; ++band) {
		bihp = bihpp[band];
		if (bihp == NULL) {
			continue;
		}

		wrote_hdr = TRUE;

		if (hwprmb(fd, BIH_HNAME, band, BIH_VERSION) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, BIH_NBYTES,
			    itoa(value, bihp->nbytes)) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, BIH_NBITS,
			    itoa(value, bihp->nbits)) == ERROR) {
			return (ERROR);
		}

		if (bihp->annot != NULL) {
			char           *s;

			for (s = walksv(bihp->annot, TRUE);
			     s != NULL;
			     s = walksv(bihp->annot, FALSE)) {
				if (hputrec(fd, (char *) NULL, BIH_ANNOT, s)
				    == ERROR) {
					return (ERROR);
				}
			}
		}

		if (! _no_hist[fd]) {
			if (bihp->history != NULL) {
				char           *s;

				for (s = walksv(bihp->history, TRUE);
				     s != NULL;
				   s = walksv(bihp->history, FALSE)) {
					if (hputrec(fd, (char *) NULL,
						    BIH_HISTORY, s)
					    == ERROR) {
						return (ERROR);
					}
				}
			}
 /*
  * append history record for current command
  */
			if (hputrec(fd, (char *) NULL, BIH_HISTORY,
				    _cmdline())
			    == ERROR) {
				return (ERROR);
			}
		}
	}
 /*
  * if we didn't write anything, then header was empty
  */
	assert(wrote_hdr);

 /*
  * success
  */
	_bih[fd] = bihpp;

	return (OK);
}
