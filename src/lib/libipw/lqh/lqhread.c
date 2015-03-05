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
**	lqhread -- read an IPW LQH header
**
** SYNOPSIS
**	#include "lqh.h"
**
**	LQH_T **
**	lqhread(
**		int        fd)		|* image file descriptor	 *|
**
** DESCRIPTION
**	lqhread reads a LQH image header from file descriptor fd.  An array
**	of LQH_T pointers is allocated, one per band.  If a band has a LQH
**	header, then an LQH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling lqhread, the caller must verify (by calling hrname())
**	that a LQH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of LQH_T pointers; else NULL if EOF or error
**
** GLOBALS ACCESSED
**
** ERRORS
**	can't allocate array of "lq" header pointers
**	"lq" header: bad band "{band}"
**	can't allocate "lq" header
**	"lq" header: key "{key}" has no value
**	"lq" header: bad key "{key}"
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "_lqh.h"

LQH_T **
lqhread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands / pixel		 */
	LQH_T         **lqhpp;		/* -> array of LQH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	lqhpp = (LQH_T **) hdralloc(nbands, sizeof(LQH_T *), fd, LQH_HNAME);
	if (lqhpp == NULL) {
		return (NULL);
	}

 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, LQH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		LQH_T          *lqhp;	/* -> current LQH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       LQH_HNAME, band);
			return (NULL);
		}

 /*
  * allocate header
  */
 /* NOSTRICT */
		lqhp = (LQH_T *) hdralloc(1, sizeof(LQH_T), fd, LQH_HNAME);
		if (lqhp == NULL) {
			return (NULL);
		}

		lqhp->maplen = ipow2(hnbits(fd, band));
		if (lqhp->maplen < 1) {
			return (NULL);
		}

		if (_lqharrays(lqhp) == ERROR) {
			return (NULL);
		}

		lqhp->nbkpts = 0;

		lqhpp[band] = lqhp;

 /*
  * ingest records
  */
		while ((err = hgetrec(fd, (char *) NULL, key, value))
		       == HGOT_DATA) {
 /*
  * ignore all-comment records
  */
			if (key[0] == EOS) {
				continue;
			}
 /*
  * barf if missing value
  */
			if (value[0] == EOS) {
				uferr(fd);
				usrerr("\"%s\" header, key \"%s\": no value",
				       LQH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, LQH_MAP)) {
				int             i;
				fpixel_t        f;

				if (lqhp->nbkpts >= lqhp->maplen) {
					usrerr(
					       "\"%s\" header: too many \"%s\" keys",
					       LQH_HNAME, key);
					return (NULL);
				}

				if (sscanf(value, "%d %f", &i, &f) != 2) {
					usrerr(
					       "\"%s\" header: key \"%s\" must have exactly 2 values",
					       LQH_HNAME, key);
					return (NULL);
				}

				if (i < 0 || i > lqhp->maplen - 1) {
					usrerr(
					       "\"%s\" header, key \"%s\": index < 0 or > %d",
					       LQH_HNAME, key,
					       lqhp->maplen - 1);
					return (NULL);
				}

				lqhp->map[i] = f;
				lqhp->bkpt[lqhp->nbkpts] = i;
				++lqhp->nbkpts;
			}
			else if (streq(key, LQH_UNITS) &&
			   (lqhp->units = strdup(value)) == NULL) {
				usrerr("insufficent memory");
				return (NULL);
			}
			else if (streq(key, LQH_INTERP) &&
			   (lqhp->interp = strdup(value)) == NULL) {
				usrerr("insufficent memory");
				return (NULL);
			}
		}

		if (err == ERROR) {
			return (NULL);
		}
	}

	if (hname == NULL) {
		return (NULL);
	}

 /*
  * verify the header
  */
	if (! _lqhcheck(lqhpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	_lqh[fd] = lqhpp;
	return (lqhpp);
}
