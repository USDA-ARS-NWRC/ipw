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
**	geohcheck -- validate components of IPW GEOH header
**
** SYNOPSIS
**	#include "_geoh.h"
**
**	bool_t
**	_geohcheck(
**		GEOH_T   **geohpp,	|* -> array of -> GEOH header	 *|
**		int        nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	geohcheck checks that geohpp points to an array of nbands pointers
**	to valid GEOH headers.
**
** RETURN VALUE
**	TRUE if geohpp checks OK, else FALSE.
*/

#include "ipw.h"
#include "geoh.h"
#include "_geoh.h"

bool_t
_geohcheck(
	GEOH_T        **geohpp,		/* -> array of -> GEOH header	 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	bool_t          found;		/* ? found at least 1 header	 */

	assert(geohpp != NULL);
 /*
  * loop through possible bands
  */
	found = FALSE;

	for (band = 0; band < nbands; ++band) {
		GEOH_T         *geohp;	/* -> GEOH for current band	 */

		geohp = geohpp[band];
		if (geohp == NULL) {
			continue;
		}

		found = TRUE;

		if (geohp->dline == 0.0) {
			usrerr(
			   "\"%s\" header, band %d: %g: bad line increment",
			       GEOH_HNAME, band, geohp->dline);
			return (FALSE);
		}

		if (geohp->dsamp == 0.0) {
			usrerr(
			 "\"%s\" header, band %d: %g: bad sample increment",
			       GEOH_HNAME, band, geohp->dsamp);
			return (FALSE);
		}

		if (geohp->units == NULL) {
			usrerr("\"%s\" header, band %d: missing units",
			       GEOH_HNAME, band);
			return (FALSE);
		}

		if (geohp->csys == NULL) {
			usrerr("\"%s\" header, band %d: missing csys",
			       GEOH_HNAME, band);
			return (FALSE);
		}
	}

	assert(found);

	return (TRUE);
}
