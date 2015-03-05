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
**	skewhread -- read an IPW SKEWH header
**
** SYNOPSIS
**	#include "skewh.h"
**
**	SKEWH_T **
**	skewhread(
**		int     fd)		|* image file descriptor	 *|
**
** DESCRIPTION
**	skewhread reads a SKEWH image header from file descriptor fd.  An array
**	of SKEWH_T pointers is allocated, one per band.  If a band has a SKEWH
**	header, then an SKEWH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling skewhread, the caller must verify (by calling hrname())
**	that a SKEWH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of SKEWH_T pointers; else NULL if EOF or error
**
** ERRORS
**	can't allocate array of "skew" header pointers
**	"skew" header: bad band "{band}"
**	can't allocate "skew" header
**	"skew" header: key "{key}" has no value
**	"skew" header: bad key "{key}"
*/

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "skewh.h"
#include "_skewh.h"

SKEWH_T **
skewhread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands / pixel		 */
	SKEWH_T       **skewhpp;	/* -> array of SKEWH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	skewhpp = (SKEWH_T **) hdralloc(nbands, sizeof(SKEWH_T *), fd,
					SKEWH_HNAME);
	if (skewhpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, SKEWH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		SKEWH_T        *skewhp;	/* -> current SKEWH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       SKEWH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		skewhp = (SKEWH_T *) hdralloc(1, sizeof(SKEWH_T), fd,
					      SKEWH_HNAME);
		if (skewhp == NULL) {
			return (NULL);
		}

		skewhpp[band] = skewhp;
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
				       SKEWH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, SKEWH_ANGLE)) {
				skewhp->angle = atof(value);
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
	if (!_skewhcheck(skewhpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	return (skewhpp);
}
