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
**	horhread -- read an IPW HORH header
**
** SYNOPSIS
**	#include "horh.h"
**
**	HORH_T **
**	horhread(
**		int             fd)	|* image file descriptor	 *|
**
** DESCRIPTION
**	Horhread reads a HORH image header from file descriptor fd.  An
**	array of HORH_T pointers is allocated, one per band.  If a band
**	has a HORH header, then a HORH_T header is allocated and its
**	address is placed in the corresponding array element;
**	otherwise, the corresponding array element is NULL.
**
**	Before calling horhread, the caller must verify (by calling
**	hrname()) that a HORH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of HORH_T pointers; else NULL if EOF or error
*/

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "horh.h"
#include "_horh.h"

HORH_T **
horhread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands			 */
	HORH_T        **horhpp;		/* -> array of HORH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	horhpp = (HORH_T **) hdralloc(nbands, sizeof(HORH_T *), fd,
				      HORH_HNAME);
	if (horhpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, HORH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		HORH_T         *horhp;	/* -> current HORH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       HORH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		horhp = (HORH_T *) hdralloc(1, sizeof(HORH_T), fd,
					    HORH_HNAME);
		if (horhp == NULL) {
			return (NULL);
		}

		horhpp[band] = horhp;
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
				       HORH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, HORH_AZM)) {
				horhp->azimuth = atof(value);
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
  * verify the header, check that azimuth is within +- pi
  */
	if (!_horhcheck(horhpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	return (horhpp);
}
