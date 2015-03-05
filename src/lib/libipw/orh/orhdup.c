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
**	orhdup -- duplicate an IPW ORH header
**
** SYNOPSIS
**	#include "orh.h"
**
**	ORH_T **
**	orhdup(
**		ORH_T         **oldhpp,	|* -> old ORH array		 *|
**		int             nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	orhdup creates a duplicate of the IPW ORH header pointed to by
**	"old".
**
**	orhdup is an easy way for applications programs to create a new ORH
**	(e.g., for an output image, after having read the ORH for an input
**	image).
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "orh.h"
#include "_orh.h"

ORH_T **
orhdup(
	ORH_T         **oldhpp,		/* -> old ORH array		 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	ORH_T         **newhpp;		/* -> new ORH array		 */

 /*
  * source ORH must be valid
  */
	assert(_orhcheck(oldhpp, nbands));
 /*
  * allocate new pointer array
  */
 /* NOSTRICT */
	newhpp = (ORH_T **) hdralloc(nbands, sizeof(ORH_T *), ERROR,
				     ORH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}
 /*
  * duplicate headers
  */
	for (band = 0; band < nbands; ++band) {
		ORH_T          *newhp;	/* -> new ORH			 */
		ORH_T          *oldhp;	/* -> new ORH			 */

		if ((oldhp = oldhpp[band]) == NULL) {
			continue;
		}
 /* NOSTRICT */
		newhp = (ORH_T *) hdralloc(1, sizeof(ORH_T), ERROR,
					   ORH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}
#if 0
 /*
  * duplicate scalar fields
  */
 /* NOSTRICT */
		bcopy((char *) oldhp, (char *) newhp, sizeof(ORH_T));
#endif
 /*
  * duplicate string fields
  */
		if (oldhp->orient != NULL
		    && (newhp->orient = strdup(oldhp->orient)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}
		if (oldhp->origin != NULL
		    && (newhp->origin = strdup(oldhp->origin)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}

		newhpp[band] = newhp;
	}

	return (newhpp);
}
