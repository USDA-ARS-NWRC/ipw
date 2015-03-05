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
**	geohwrite -- write an IPW GEOH header
**
** SYNOPSIS
**	#include "file.h"
**
**	int
**	geohwrite(
**		int             fd,	  |* output file descriptor	 *|
**		GEOH_T        **geohpp)	  |* -> array of GEOH pointers	 *|
**
** DESCRIPTION
**      geohwrite writes the array of GEOH headers pointed to by geohpp
**	to file descriptor fd.
**
**	geohwrite is called by IPW application programs to write
**	GEOH headers.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

/* Oct 2, 1991, Kelly Longley - changed calls to ftoa to dtoa.  Needed
   more digits. */

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "geoh.h"
#include "_geoh.h"

int
geohwrite(
	int             fd,		/* output file descriptor	 */
	GEOH_T        **geohpp)		/* -> array of GEOH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */

	nbands = hnbands(fd);

	assert(_geohcheck(geohpp, nbands));
 /*
  * loop through possible output bands
  */
	for (band = 0; band < nbands; ++band) {
		REG_1 GEOH_T   *geohp;	/* -> current GEOH		 */
		char            s[HREC_SIZ];	/* output value		 */

		geohp = geohpp[band];
		if (geohp == NULL) {
			continue;
		}
 /*
  * write preamble
  */
		if (hwprmb(fd, GEOH_HNAME, band, GEOH_VERSION) == ERROR) {
			return (ERROR);
		}
 /*
  * write fields
  */
		if (hputrec(fd, (char *) NULL, GEOH_BLINE,
			    dtoa(s, geohp->bline)) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, GEOH_BSAMP,
			    dtoa(s, geohp->bsamp)) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, GEOH_DLINE,
			    dtoa(s, geohp->dline)) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, GEOH_DSAMP,
			    dtoa(s, geohp->dsamp)) == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, GEOH_UNITS, geohp->units)
		    == ERROR) {
			return (ERROR);
		}

		if (hputrec(fd, (char *) NULL, GEOH_CSYS, geohp->csys)
		    == ERROR) {
			return (ERROR);
		}
	}

	return (OK);
}
