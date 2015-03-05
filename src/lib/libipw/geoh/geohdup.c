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
**	geohdup -- duplicate an IPW GEOH header
**
** SYNOPSIS
**	#include "geoh.h"
**
**	GEOH_T **name(old)
**	GEOH_T **old;
**
** DESCRIPTION
**	geohdup creates a duplicate of the IPW GEOH header pointed to by
**	"old".
**
** RESTRICTIONS
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
**
** GLOBALS ACCESSED
**
** ERRORS
**
** WARNINGS
**
** APPLICATION USAGE
**	geohdup is an easy way for applications programs to create a new GEOH
**	(e.g., for an output image, after having read the GEOH for an input
**	image).
**
** FUTURE DIRECTIONS
**
** BUGS
*/

#include <string.h>

#include "ipw.h"
#include "geoh.h"
#include "hdrio.h"
#include "_geoh.h"

GEOH_T **
geohdup(
	GEOH_T        **oldhpp,		/* -> old GEOH array		 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	GEOH_T        **newhpp;		/* -> new GEOH array		 */

 /*
  * source GEOH must be valid
  */
	assert(_geohcheck(oldhpp, nbands));

 /*
  * allocate new pointer array
  */
 /* NOSTRICT */
	newhpp = (GEOH_T **) hdralloc(nbands, sizeof(GEOH_T *), ERROR,
				      GEOH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}
 /*
  * duplicate headers
  */
	for (band = 0; band < nbands; ++band) {
		GEOH_T         *newhp;	/* -> new GEOH			 */
		GEOH_T         *oldhp;	/* -> new GEOH			 */

		oldhp = oldhpp[band];
 /* NOSTRICT */
		newhp = (GEOH_T *) hdralloc(1, sizeof(GEOH_T), ERROR,
					    GEOH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}
 /*
  * duplicate scalar fields
  */
 /* NOSTRICT */
		bcopy((char *) oldhp, (char *) newhp, sizeof(GEOH_T));
 /*
  * duplicate string fields
  */
		if (oldhp->units != NULL
		    && (newhp->units = strdup(oldhp->units)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}

		if (oldhp->csys != NULL
		    && (newhp->csys = strdup(oldhp->csys)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}

		newhpp[band] = newhp;
	}

	return (newhpp);
}
