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
**	geohread -- read an IPW GEOH header
**
** SYNOPSIS
**	#include "geoh.h"
**
**	GEOH_T **
**	geohread(
**		int fd)
**
** DESCRIPTION
**	geohread reads a GEOH image header from file descriptor fd.  An array
**	of GEOH_T pointers is allocated, one per band.  If a band has a GEOH
**	header, then an GEOH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling geohread, the caller must verify (by calling hrname())
**	that a GEOH header is available for ingesting.
**
**	geohread is called by application programs to ingest GEOH headers.
**
** RETURN VALUE
**	pointer to array of GEOH_T pointers; else NULL if EOF or error
**
** ERRORS
**	can't allocate array of "geo" header pointers
**	"geo" header: bad band "{band}"
**	can't allocate "geo" header
**	"geo" header: key "{key}" has no value
**	"geo" header: bad key "{key}"
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "geoh.h"
#include "_geoh.h"

GEOH_T **
geohread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands			 */
	GEOH_T        **geohpp;		/* -> array of GEOH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	geohpp = (GEOH_T **) hdralloc(nbands, sizeof(GEOH_T *), fd,
				      GEOH_HNAME);
	if (geohpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, GEOH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		GEOH_T         *geohp;	/* -> current GEOH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       GEOH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		geohp = (GEOH_T *) hdralloc(1, sizeof(GEOH_T), fd,
					    GEOH_HNAME);
		if (geohp == NULL) {
			return (NULL);
		}

		geohpp[band] = geohp;
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
				       GEOH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, GEOH_BLINE)) {
				geohp->bline = atof(value);
			}
			else if (streq(key, GEOH_BSAMP)) {
				geohp->bsamp = atof(value);
			}
			else if (streq(key, GEOH_DLINE)) {
				geohp->dline = atof(value);
			}
			else if (streq(key, GEOH_DSAMP)) {
				geohp->dsamp = atof(value);
			}
			else if (streq(key, GEOH_UNITS) &&
				 (geohp->units = strdup(value)) == NULL) {
				usrerr("insufficient memory");
				return (NULL);
			}
			else if (streq(key, GEOH_CSYS) &&
				 (geohp->csys = strdup(value)) == NULL) {
				usrerr("insufficient memory");
				return (NULL);
			}
 /*
  * ignore unrecognized keys
  */
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
	if (!_geohcheck(geohpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	return (geohpp);
}
