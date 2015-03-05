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
**	winhread -- read an IPW WINH header
**
** SYNOPSIS
**	#include "winh.h"
**
**	WINH_T **
**	winhread(
**		int             fd)	|* image file descriptor	 *|
**
** DESCRIPTION
**	winhread reads a WINH image header from file descriptor fd.  An array
**	of WINH_T pointers is allocated, one per band.  If a band has a WINH
**	header, then an WINH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling winhread, the caller must verify (by calling hrname())
**	that a WINH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of WINH_T pointers; else NULL if EOF or error
**
** ERRORS
**	can't allocate array of "win" header pointers
**	"win" header: bad band "{band}"
**	can't allocate "win" header
**	"win" header: key "{key}" has no value
**	"win" header: bad key "{key}"
*/

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "winh.h"
#include "_winh.h"

WINH_T **
winhread(
	int             fd)		/* image file descriptor	 */
{
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands			 */
	WINH_T        **winhpp;		/* -> array of WINH pointers	 */

 /*
  * allocate array of header pointers
  */
	nbands = hnbands(fd);
	assert(nbands > 0);

 /* NOSTRICT */
	winhpp = (WINH_T **) hdralloc(nbands, sizeof(WINH_T *), fd, WINH_HNAME);
	if (winhpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, WINH_HNAME)) {
		int             band;	/* current header band #	 */
		int             err;	/* hgetrec return value		 */
		WINH_T         *winhp;	/* -> current WINH		 */

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       WINH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		winhp = (WINH_T *) hdralloc(1, sizeof(WINH_T), fd, WINH_HNAME);
		if (winhp == NULL) {
			return (NULL);
		}

		winhpp[band] = winhp;
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
				       WINH_HNAME, key);
				return (NULL);
			}
 /*
  * match key to header field, ingest value
  */
			if (streq(key, WINH_BLINE)) {
				winhp->bline = atof(value);
			}
			else if (streq(key, WINH_BSAMP)) {
				winhp->bsamp = atof(value);
			}
			else if (streq(key, WINH_DLINE)) {
				winhp->dline = atof(value);
			}
			else if (streq(key, WINH_DSAMP)) {
				winhp->dsamp = atof(value);
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
	if (!_winhcheck(winhpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	return (winhpp);
}
