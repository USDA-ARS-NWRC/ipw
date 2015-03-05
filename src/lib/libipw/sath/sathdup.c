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
**	sathdup -- duplicate an IPW SATH header
**
** SYNOPSIS
**	#include "sath.h"
**
**	SATH_T **
**	sathdup(
**		SATH_T        **oldhpp,	  |* -> old SATH array	 	*|
**		int             nbands)	  |* # header bands		*|
**
** DESCRIPTION
**	sathdup creates a duplicate of the IPW SATH header pointed to by
**	"oldhpp".
**
**	sathdup is an easy way for applications programs to create a new SATH
**	(e.g., for an output image, after having read the SATH for an input
**	image).
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "sath.h"
#include "_sath.h"

SATH_T **
sathdup(
	SATH_T        **oldhpp,		/* -> old SATH array	 	*/
	int             nbands)		/* # header bands		*/
{
	int             band;		/* loop counter			 */
	SATH_T        **newhpp;		/* -> new SATH array	 */

 /*
  * source SATH must be valid
  */
	assert(_sathcheck(oldhpp, nbands));

 /*
  * allocate new pointer array
  */
 /* NOSTRICT */
	newhpp = (SATH_T **) hdralloc(nbands, sizeof(SATH_T *), ERROR,
				      SATH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}

 /*
  * duplicate headers
  */
	for (band = 0; band < nbands; ++band) {
		SATH_T         *newhp;	/* -> new SATH			 */
		SATH_T         *oldhp;	/* -> new SATH			 */

		oldhp = oldhpp[band];
 /* NOSTRICT */
		newhp = (SATH_T *) hdralloc(1, sizeof(SATH_T), ERROR,
					    SATH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}
 /*
  * duplicate string fields
  */
		if (oldhp->platform != NULL
		    && (newhp->platform = strdup(oldhp->platform)) == NULL) {
			return (NULL);
		}
		if (oldhp->sensor != NULL
		    && (newhp->sensor = strdup(oldhp->sensor)) == NULL) {
			return (NULL);
		}
		if (oldhp->location != NULL
		    && (newhp->location = strdup(oldhp->location)) == NULL) {
			return (NULL);
		}
		if (oldhp->gmdate != NULL
		    && (newhp->gmdate = strdup(oldhp->gmdate)) == NULL) {
			return (NULL);
		}
		if (oldhp->gmtime != NULL
		    && (newhp->gmtime = strdup(oldhp->gmtime)) == NULL) {
			return (NULL);
		}

		newhpp[band] = newhp;
	}

	return (newhpp);
}
