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
**	hputrec -- encode and write a header record
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	int
**	hputrec(
**		int             fd,	    |* output file descriptor	 *|
**		const char     *comment,    |* -> comment string	 *|
**		const char     *key,	    |* -> keyword string	 *|
**		const char     *value)	    |* -> value string		 *|
**
** DESCRIPTION
**	hputrec creates a header record and writes it to fd.  If key is
**	non-NULL, then a key-value pair is placed in the record, using the key
**	string pointed to by key and the value string pointed to by value.  If
**	comment is non-NULL, then the string pointed to by comment is written
**	as a comment at the end of the record.
**
**	Hputrec is meant to be called by header-specific output functions.
**
** RESTRICTIONS
**	If key is non-NULL, then value must be non-NULL.  If key is NULL,
**	then value must be NULL.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**
** ERRORS
**	header record key+value fields too long
**	header record comment field too long
**		The specified field(s) wouldn't fit in a header record.
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_hdrio.h"

int
hputrec(
	int             fd,		/* output file descriptor	 */
	const char     *comment,	/* -> comment string		 */
	const char     *key,		/* -> keyword string		 */
	const char     *value)		/* -> value string		 */
{
	REG_1 char     *bufp;		/* -> current position in buf	 */
	REG_2 char     *eobuf;		/* -> 1 past last data char in buf */

	char            buf[HREC_MAX + 1];	/* record buffer	 */

	bufp = buf;
	eobuf = bufp + sizeof(buf) - 2;
 /*
  * key + value
  */
	if (key == NULL) {
		assert(value == NULL);
	}
	else {
		assert(value != NULL);

		if (strlen(key) + strlen(value) + 4 > (eobuf - bufp)) {
			uferr(fd);
			usrerr("header record key+value fields too long");
			return (ERROR);
		}

		(void) sprintf(bufp, "%s %c %s ", key, KEY_VAL_SEP, value);
		bufp += strlen(bufp);
	}
 /*
  * comment
  */
	if (comment != NULL) {
		if (strlen(comment) + 2 > (eobuf - bufp)) {
			uferr(fd);
			usrerr("header record comment field too long");
			return (ERROR);
		}

		(void) sprintf(bufp, "%c %s ", BEGIN_COMMENT, comment);
		bufp += strlen(bufp);
	}
 /*
  * trailing \n\0
  */
	*bufp++ = '\n';
	*bufp = EOS;

	return (_hwrec(fd, buf));
}
