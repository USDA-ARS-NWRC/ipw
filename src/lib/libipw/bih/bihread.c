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
**	bihread -- read an IPW BIH header
**
** SYNOPSIS
**	#include "bih.h"
**
**	BIH_T **
**	bihread(
**		int     fd)		|* image file descriptor	 *|
**
** DESCRIPTION
**	bihread reads a BIH image header from file descriptor fd.  An array
**	of BIH_T pointers is allocated, one per band.  If a band has a BIH
**	header, then an BIH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling bihread, the caller must verify (by calling hrname())
**	that a BIH header is available for ingesting.
**
**	bihread is called by application programs to ingest BIH headers.
**
** RETURN VALUE
**	pointer to array of BIH_T pointers; else NULL if EOF or error
**
** GLOBALS ACCESSED
**
** ERRORS
**	can't allocate array of "{name}" header pointers
**	"{name}" header: bad band "{band}"
**	can't allocate "{name}" header
**	"{name}" header: key "{key}" has no value
**	"{name}" header: bad key "{key}"
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_bih.h"

BIH_T **
bihread(
	int             fd)		/* image file descriptor	 */
{
	int             err;		/* hgetrec return value		 */
	int             band;		/* current header band #	 */
	char           *hname;		/* current header name		 */
	int             nbands;		/* # bands			 */
	BIHI_T         *bihip;		/* -> BIH pof omage component	 */
	BIH_T         **bihpp;		/* -> array of BIH pointers	 */

	char            key[HREC_MAX + 1];	/* keyword		 */
	char            value[HREC_MAX + 1];	/* value string		 */

 /*
  * verify that BIH is waiting to be read
  */
	hname = hrname(fd);
	if (hname == NULL) {
		return (NULL);
	}

	if (strdiff((hname), BIHI_HNAME)) {
		uferr(fd);
		usrerr("invalid IPW image (no \"%s\" header)", BIHI_HNAME);
		return (NULL);
	}
 /*
  * allocate per-image component
  */
 /* NOSTRICT */
	bihip = (BIHI_T *) hdralloc(1, sizeof(BIHI_T), fd, BIHI_HNAME);
	if (bihip == NULL) {
		return (NULL);
	}
 /*
  * ingest records
  */
	while ((err = hgetrec(fd, (char *) NULL, key, value)) == HGOT_DATA) {
		if (key[0] == EOS) {
			continue;
		}

		if (value[0] == EOS) {
			uferr(fd);
			usrerr("\"%s\" header: key \"%s\" has no value",
			       BIHI_HNAME, key);
			return (NULL);
		}

		if (streq(key, BIH_BYTEORDER) &&
		    (bihip->byteorder = strdup(value)) == NULL) {
			usrerr("insufficient memory");
			return (NULL);
		}
		else if (streq(key, BIH_NLINES)) {
			bihip->nlines = (int) strtol(value, (char **)NULL, 10);
		}
		else if (streq(key, BIH_NSAMPS)) {
			bihip->nsamps = (int) strtol(value, (char **)NULL, 10);
		}
		else if (streq(key, BIH_NBANDS)) {
			bihip->nbands = (int) strtol(value, (char **)NULL, 10);
		}
 /*
  * ignore unrecognized keys
  */
	}

	if (err == ERROR) {
		return (NULL);
	}
 /*
  * allocate array of header pointers
  */
	nbands = bihip->nbands;
	if (nbands < 1) {
		uferr(fd);
		usrerr("\"%s\" header: nbands < 1", BIHI_HNAME);
		return (NULL);
	}

 /* NOSTRICT */
	bihpp = (BIH_T **) hdralloc(nbands, sizeof(BIH_T *), fd, BIH_HNAME);
	if (bihpp == NULL) {
		return (NULL);
	}
 /*
  * loop through per-band headers
  */
	while ((hname = hrname(fd)) != NULL && streq(hname, BIH_HNAME)) {
		BIH_T          *bihp;	/* -> current BIH		 */

 /*
  * get header band #
  */
		band = hrband(fd);
		if (band < 0 || band > nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
			       BIH_HNAME, band);
			return (NULL);
		}
 /*
  * allocate header
  */
 /* NOSTRICT */
		bihp = (BIH_T *) hdralloc(1, sizeof(BIH_T), fd, BIH_HNAME);
		if (bihp == NULL) {
			return (NULL);
		}

		bihpp[band] = bihp;
 /*
  * ingest records
  */
		while ((err = hgetrec(fd, (char *) NULL, key, value))
		       == HGOT_DATA) {
			if (key[0] == EOS) {
				continue;
			}

			if (value[0] == EOS) {
				uferr(fd);
				usrerr("\"%s\" header, key \"%s\": no value",
				       BIH_HNAME, key);
				return (NULL);
			}

			if (streq(key, BIH_NBYTES)) {
				bihp->nbytes = (int) strtol(value,
							    (char **)NULL, 10);
			}
			else if (streq(key, BIH_NBITS)) {
				bihp->nbits = (int) strtol(value,
							   (char **)NULL, 10);
			}
			else if (streq(key, BIH_ANNOT)) {
				bihp->annot = addsv(bihp->annot, value);
				if (bihp->annot == NULL) {
					uferr(fd);
		usrerr("\"%s\" header, key \"%s\": can't ingest \"%s\"",
					       BIH_HNAME, key, value);
					return (NULL);
				}
			}
			else if (streq(key, BIH_HISTORY)) {
				bihp->history = addsv(bihp->history, value);
				if (bihp->history == NULL) {
					uferr(fd);
		usrerr("\"%s\" header, key \"%s\": can't ingest \"%s\"",
					       BIH_HNAME, key, value);
					return (NULL);
				}
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
  * make sure we got a BIH for each band
  */
	for (band = 0; band < nbands; ++band) {
		if (bihpp[band] == NULL) {
			uferr(fd);
			usrerr("no \"%s\" header for band %d",
			       BIH_HNAME, band);
			return (NULL);
		}

		bihpp[band]->img = bihip;
	}
 /*
  * verify the header
  */
	if (! _bihcheck(bihpp)) {
		uferr(fd);
		return (NULL);
	}
 /*
  * done: return -> array of -> per-band headers
  */
	_bih[fd] = bihpp;

	return (bihpp);
}
