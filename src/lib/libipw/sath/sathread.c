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
**	sathread -- read an IPW SATH header
**
** SYNOPSIS
**	#include "sath.h"
**
**	SATH_T **
**	sathread(
**		int     fd)		|* image file descriptor	 *|
**
** DESCRIPTION
**	sathread reads a SATH image header from file descriptor fd.  An array
**	of SATH_T pointers is allocated, one per band.  If a band has a SATH
**	header, then an SATH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling sathread, the caller must verify (by calling hrname())
**	that a SATH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of SATH_T pointers; else NULL if EOF or error
**
** ERRORS
**	can't allocate array of "sat" header pointers
**	"sat" header: bad band "{band}"
**	can't allocate "sat" header
**	"sat" header: key "{key}" has no value
**	"sat" header: bad key "{key}"
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "sath.h"
#include "_sath.h"

SATH_T **
sathread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands / pixel		 */
	SATH_T        **sathpp;		/* -> array of SATH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	sathpp = (SATH_T **) hdralloc(nbands, sizeof(SATH_T *), fd,
				      SATH_HNAME);
	if (sathpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, SATH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		SATH_T         *sathp;	/* -> current SATH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       SATH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		sathp = (SATH_T *) hdralloc(1, sizeof(SATH_T), fd,
					    SATH_HNAME);
		if (sathp == NULL) {
			return (NULL);
		}

		sathpp[band] = sathp;
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
				       SATH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, SATH_PLATFORM)
			    && (sathp->platform = strdup(value)) == NULL) {
				return (NULL);
			}
			else if (streq(key, SATH_SENSOR)
				 && (sathp->sensor = strdup(value)) == NULL) {
				return (NULL);
			}
			else if (streq(key, SATH_LOCATION)
				 && (sathp->location = strdup(value)) == NULL) {
				return (NULL);
			}
			else if (streq(key, SATH_GMDATE)
				 && (sathp->gmdate = strdup(value)) == NULL) {
				return (NULL);
			}
			else if (streq(key, SATH_GMTIME)
				 && (sathp->gmtime = strdup(value)) == NULL) {
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
	if (!_sathcheck(sathpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	return (sathpp);
}
