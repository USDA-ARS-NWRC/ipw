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
**	_hrrec -- read a header-record
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	int
**	_hrrec(
**		int     fd,
**		char   *buf,
**		int     bufsiz)
**
** DESCRIPTION
**	_hrrec reads the next image header record from file descriptor fd
**	into the bufsiz-character array pointed to by buf.
**
** RESTRICTIONS
**	bufsiz should be at least HREC_MAX+1 (i.e. longest allowable header
**	record plus terminating EOS).
**
** RETURN VALUE
**	HGOT_DATA	if a header record was successfully copied into buf
**	HGOT_PRMB	if a header preamble was encountered
**	ERROR		bad preamble; malloc failed; error/EOF on fd; etc.
**
** GLOBALS ACCESSED
**	_hdriocb
**
** ERRORS
**	image header read error
**	can't allocate image header {name,version} string
**
** WARNINGS
**	If _hrrec returns HGOT_PRMB, then the array pointed to by buf will
**	contain garbage (it is used as a workspace for parsing the preamble).
**
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

#define	field_len(p)	strcspn(p, WHITE_SPACE)
#define	space_len(p)	strspn(p, WHITE_SPACE)

int
_hrrec(
	int             fd,
	char           *buf,
	int             bufsiz)
{
	if (ugets(fd, buf, bufsiz) == NULL) {
		uferr(fd);
		usrerr("image header read error");
		return (ERROR);
	}

	if (ISPRMB(buf)) {
 /*
  * header preamble record
  */
		REG_1 char     *bufp;	/* -> current pos in buf */
		REG_2 char     *eofield;/* -> end of current field */
		REG_3 HDRIO_T  *p;	/* -> hdrio ctrl block	 */

		p = &_hdriocb[fd];

		SAFE_FREE(p->name);
		SAFE_FREE(p->version);

		bufp = buf;
 /*
  * skip magic string + subsequent white space
  */
		bufp += field_len(bufp);
		bufp += space_len(bufp);
 /*
  * name
  */
		if (bufp[1] == BLANK) {
 /*
  * OLD HEADER: 2nd char is embedded blank
  */
			eofield = bufp + 2;
			eofield += field_len(eofield);
		}
		else {
			eofield = bufp + field_len(bufp);
		}

		*eofield = EOS;

		p->name = strdup(bufp);
		if (p->name == NULL) {
			uferr(fd);
			usrerr("can't allocate image header name string");
			return (ERROR);
		}

		bufp = eofield + 1;
		bufp += space_len(bufp);
 /*
  * band
  */
		if (*bufp == RCS_KDELIM) {
 /*
  * OLD HEADER: we've already reached the version field, so the band field
  * must have been blank -> this is a per-image header
  */
			p->band = NO_BAND;
		}
		else {
			eofield = bufp + field_len(bufp);
			*eofield = EOS;

			p->band = atoi(bufp);

			bufp = eofield + 1;
			bufp += space_len(bufp);
		}
 /*
  * version
  */
		eofield = strchr(bufp, BEGIN_COMMENT);
		if (eofield != NULL) {
 /*
  * OLD HEADER: last char before '\n' is a '#': get rid of it
  */
			*eofield = EOS;
		}

		rmtrail(bufp);

		p->version = strdup(bufp);
		if (p->version == NULL) {
			uferr(fd);
			usrerr("can't allocate image header version string");
			return (ERROR);
		}

		return (HGOT_PRMB);
	}
 /*
  * header data record
  */
	return (HGOT_DATA);
}
