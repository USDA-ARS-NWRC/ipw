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
**	bihdup -- duplicate an IPW basic image header
**
** SYNOPSIS
**	#include "bih.h"
**
**	BIH_T **
**	bihdup(
**		BIH_T         **oldhpp)	    |* -> old BIH array		 *|
**
** DESCRIPTION
**	Bihdup creates a duplicate of the IPW basic image header pointed to
**	by "old".
**
**	Bihdup is an easy way for applications programs to create a new BIH
**	(e.g., for an output image, after having read the BIH for an input
**	image).
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
**
** GLOBALS ACCESSED
**
** ERRORS
**	can't duplicate BIH "byteorder" string
*/

#include <string.h>
#include <strings.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"

BIH_T **
bihdup(
	BIH_T         **oldhpp)		/* -> old BIH array		 */
{
	int             band;		/* loop counter			 */
	int             nbands;		/* # bands in header		 */
	BIHI_T         *newhip;		/* -> new per-image component	 */
	BIH_T         **newhpp;		/* -> new BIH array		 */
	BIHI_T         *oldhip;		/* -> old per-image component	 */

 /*
  * source BIH must be valid
  */
	assert(_bihcheck(oldhpp));

 /*
  * duplicate per-image component
  */
	oldhip = oldhpp[0]->img;

 /* NOSTRICT */
	newhip = (BIHI_T *) hdralloc(1, sizeof(BIHI_T), ERROR, BIHI_HNAME);
	if (newhip == NULL) {
		return (NULL);
	}

 /* NOSTRICT */
	bcopy((char *) oldhip, (char *) newhip, sizeof(BIHI_T));

	newhip->byteorder = strdup(o_byteorder);
	if (newhip->byteorder == NULL) {
		usrerr("insufficent memory");
		return (NULL);
	}

 /*
  * create new pointer array
  */
	nbands = bih_nbands(oldhpp[0]);

 /* NOSTRICT */
	newhpp = (BIH_T **) hdralloc(nbands, sizeof(BIH_T *), ERROR,
				     BIH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}
 /*
  * duplicate per-band component(s)
  */
	for (band = 0; band < nbands; ++band) {
		BIH_T          *newhp;
		BIH_T          *oldhp;

		oldhp = oldhpp[band];
 /* NOSTRICT */
		newhp = (BIH_T *) hdralloc(1, sizeof(BIH_T), ERROR,
					   BIH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}

		newhp->img = newhip;
		newhp->nbytes = oldhp->nbytes;
		newhp->nbits = oldhp->nbits;

		if (oldhp->annot != NULL) {
			newhp->annot = dupsv(oldhp->annot);
			if (newhp->annot == NULL) {
				return (NULL);
			}
		}

		if (oldhp->history != NULL) {
			newhp->history = dupsv(oldhp->history);
			if (newhp->history == NULL) {
				return (NULL);
			}
		}

		newhpp[band] = newhp;
	}

	return (newhpp);
}
