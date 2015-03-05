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
**	lqhdup -- duplicate an IPW LQH header
**
** SYNOPSIS
**	#include "lqh.h"
**
**	LQH_T **
**	lqhdup(
**		LQH_T  **oldhpp,	|* -> old LQH array		 *|
**		int      nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	lqhdup creates a duplicate of the IPW LQH header pointed to by
**	"old".
**
**	lqhdup is an easy way for applications programs to create a new LQH
**	(e.g., for an output image, after having read the LQH for an input
**	image).
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_lqh.h"

LQH_T **
lqhdup(
	LQH_T         **oldhpp,		/* -> old LQH array		 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	LQH_T         **newhpp;		/* -> new LQH array		 */

 /*
  * source LQH must be valid
  */
	assert(_lqhcheck(oldhpp, nbands));

 /*
  * allocate new pointer array
  */
 /* NOSTRICT */
	newhpp = (LQH_T **) hdralloc(nbands, sizeof(LQH_T *), ERROR,
				     LQH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}

 /*
  * duplicate headers
  */
	for (band = 0; band < nbands; ++band) {
		LQH_T          *newhp;	/* -> new LQH			 */
		LQH_T          *oldhp;	/* -> new LQH			 */

		oldhp = oldhpp[band];
 /* NOSTRICT */
		newhp = (LQH_T *) hdralloc(1, sizeof(LQH_T), ERROR,
					   LQH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}

 /*
  * duplicate scalar fields
  */
 /* NOSTRICT */
		bcopy((char *) oldhp, (char *) newhp, sizeof(LQH_T));

 /*
  * duplicate string fields
  */
		if (oldhp->units != NULL &&
		    (newhp->units = strdup(oldhp->units)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}
		if (oldhp->interp != NULL &&
		    (newhp->interp = strdup(oldhp->interp)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}

 /*
  * duplicate arrays
  */
		if (_lqharrays(newhp) == ERROR) {
			return (NULL);
		}

 /* NOSTRICT */
		(void) memcpy((char *) newhp->bkpt, (char *) oldhp->bkpt,
			      (size_t) newhp->nbkpts * sizeof(int));

 /* NOSTRICT */
		(void) memcpy((char *) newhp->map, (char *) oldhp->map,
			      (size_t) newhp->maplen * sizeof(fpixel_t));
 /*
  * done with this band's header
  */
		newhpp[band] = newhp;
	}

	return (newhpp);
}
