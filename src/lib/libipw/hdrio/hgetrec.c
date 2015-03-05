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
**	hgetrec -- read and decode header record
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	int
**	hgetrec(
**		int     fd,		|* input file descriptor	 *|
**		char   *comment,	|* -> comment string		 *|
**		char   *key,		|* -> keyword string		 *|
**		char   *value)		|* -> value string		 *|
**
** DESCRIPTION
**	hgetrec reads a header record from file descriptor fd.  The comment,
**	key, and value fields of the record are copied into the arrays pointed
**	to by comment, key, and value, respectively.
**
**	All extracted fields have trailing blanks stripped and are
**	EOS-terminated.
**
** RESTRICTIONS
**	The arrays pointed to by comment, key, and value must each have
**	enough space for (HREC_MAX + 1) characters.
**
** RETURN VALUE
**	HGOT_DATA	success
**	HGOT_PRMB	a header preamble was encountered
**	ERROR		bad header record; error/EOF on fd; etc.
**
** GLOBALS ACCESSED
**
** DIAGNOSTICS
**	header record field 1 is neither key nor comment
**	header record has key without value
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

/* ------------------------------------------------------------------------ */

static int
get_comment(
	char           *comment,
	char           *bufp)
{
 /*
  * skip BEGIN_COMMENT char
  */
	++bufp;
 /*
  * strip leading/trailing white space
  */
	bufp = rmlead(bufp);
	rmtrail(bufp);
 /*
  * copy comment
  */
	(void) strcpy(comment, bufp);

	return (HGOT_DATA);
}

/* ------------------------------------------------------------------------ */

int
hgetrec(
	int   	 	fd,		/* input file descriptor	 */
	char   	       *comment,	/* -> comment string		 */
	char   	       *key,		/* -> keyword string		 */
	char	       *value)		/* -> value string		 */
{
	char           *bufp;		/* -> current position in buf	 */
	char           *eofield;	/* -> end of current field	 */
	int             err;		/* function error return	 */

	char            buf[HREC_MAX + 1];	/* header record buffer	 */

 /*
  * read header record
  */
	err = _hrrec(fd, buf, sizeof(buf));
	if (err == HGOT_PRMB || err == ERROR) {
		return (err);
	}

	assert(err == HGOT_DATA);
 /*
  * intialize output strings
  */
	if (comment != NULL) {
		*comment = EOS;
	}

	if (key != NULL) {
		*key = EOS;
	}

	if (value != NULL) {
		*value = EOS;
	}
 /*
  * everything following a BEGIN_COMMENT is a comment
  */
	bufp = rmlead(buf);

	if (*bufp == BEGIN_COMMENT) {
		if (comment != NULL) {
			return (get_comment(comment, bufp));
		}

		return (HGOT_DATA);
	}
 /*
  * first field is a keyword, so must be followed by a KEY_VAL_SEP char
  */
	eofield = strchr(bufp, KEY_VAL_SEP);
	if (eofield == NULL) {
		uferr(fd);
		usrerr("header record field 1 is neither key nor comment");
		return (ERROR);
	}
 /*
  * save keyword (note that eofield now -> beginning of next field)
  */
	*eofield++ = EOS;
	rmtrail(bufp);

	if (key != NULL) {
		(void) strcpy(key, bufp);
	}
 /*
  * must be non-comment data following KEY_VAL_SEP
  */
	bufp = rmlead(eofield);
	if (*bufp == BEGIN_COMMENT || *bufp == EOS) {
		uferr(fd);
		usrerr("header record has key without value");
		return (ERROR);
	}
 /*
  * if comment follows value, then get comment and put EOS between value and
  * comment
  */
	eofield = strchr(bufp, BEGIN_COMMENT);
	if (eofield != NULL) {
		if (comment != NULL) {
			(void) get_comment(comment, eofield);
		}

		*eofield = EOS;
	}
 /*
  * save value
  */
	rmtrail(bufp);

	if (value != NULL) {
		(void) strcpy(value, bufp);
	}

	return (HGOT_DATA);
}
