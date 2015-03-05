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
**	bihmake -- make an IPW basic image header
**
** SYNOPSIS
**	#include "bih.h"
**
**	BIH_T *
**	bihmake(
**		int        nbytes,	|* # bytes / pixel		 *|
**		int        nbits,	|* # bits / pixel		 *|
**		STRVEC_T   *history,	|* -> history strings	 	 *|
**		STRVEC_T   *annot,	|* -> annotation strings	 *|
**		BIH_T      *oldp,	|* ? get per-img stuff here	 *|
**		int         nlines,	|* # lines			 *|
**		int         nsamps,	|* # samples / line		 *|
**		int         nbands)	|* # bands			 *|
**
** DESCRIPTION
**	Bihmake allocates a single BI header.  The header is
**	initialized with the remaining arguments.  If nbytes is zero,
**	it will be made large enough to contain nbits.  If nbits is
**	zero, it will be set to CHAR_BIT * nbytes.  If bihp is null,
**	then the per-image portion of the header is initialized from
**	nlines, nsamps, and nbands.  Otherwise (bihp not NULL), a link
**	is made to the per-image portion of *bihp.
**
** RETURN VALUE
**	pointer to new BI header; NULL if error
*/

#include <string.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"

BIH_T *
bihmake(
	int             nbytes,		/* # bytes / pixel		 */
	int             nbits,		/* # bits / pixel		 */
	STRVEC_T       *history,	/* -> history strings	 	 */
	STRVEC_T       *annot,		/* -> annotation strings	 */
	BIH_T          *oldp,		/* ? get per-img stuff here	 */
	int             nlines,		/* # lines			 */
	int             nsamps,		/* # samples / line		 */
	int             nbands)		/* # bands			 */
{
	BIH_T          *bihp;		/* -> new BIH			 */

	assert(nbytes > 0 || nbits > 0);
	assert(nbytes >= 0 && nbits >= 0);

 /*
  * nbits given, derive nbytes
  */
	if (nbytes == 0) {
		nbytes = (nbits + CHAR_BIT - 1) / CHAR_BIT;
	}

 /*
  * nbytes given, derive nbits
  */
	else if (nbits == 0) {
		nbits = nbytes * CHAR_BIT;
	}

 /*
  * nbytes,nbits given
  */
	else {
		assert(nbits <= nbytes * CHAR_BIT);
	}

 /*
  * allocate new BIH
  */
 /* NOSTRICT */
	bihp = (BIH_T *) hdralloc(1, sizeof(BIH_T), ERROR, BIH_HNAME);
	if (bihp == NULL) {
		return (NULL);
	}

 /*
  * initialize per-band stuff
  */
	bihp->nbytes = nbytes;
	bihp->nbits = nbits;

	if (history != NULL) {
		bihp->history = dupsv(history);
		if (bihp->history == NULL) {
			usrerr("can't duplicate history strings");
		}
	}

	if (annot != NULL) {
		bihp->annot = dupsv(annot);
		if (bihp->annot == NULL) {
			usrerr("can't duplicate annotation strings");
		}
	}

 /*
  * if old header provided then link to its per-image stuff
  */
	if (oldp != NULL) {
		assert(oldp->img != NULL);
		bihp->img = oldp->img;
	}

 /*
  * else if no old header then initialize per-image stuff
  */
	else {
		assert(nlines > 0);
		assert(nsamps > 0);
		assert(nbands > 0);
 /* NOSTRICT */
		bihp->img = (BIHI_T *) hdralloc(1, sizeof(BIHI_T),
						ERROR, BIHI_HNAME);
		if (bihp->img == NULL) {
			return (NULL);
		}

		bihp->img->nlines = nlines;
		bihp->img->nsamps = nsamps;
		bihp->img->nbands = nbands;

		bihp->img->byteorder = strdup(o_byteorder);
		if (bihp->img->byteorder == NULL) {
			usrerr("insufficent memory");
			return (NULL);
		}
	}

	return (bihp);
}
