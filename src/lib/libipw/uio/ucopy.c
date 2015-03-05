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
**	ucopy -- copy between open files
**
** SYNOPSIS
**	#include "ipw.h"
**
***	long
***	ucopy(
***		int	      fdi,	|* input file descriptor	 *|
***		int           fdo,	|* output file descriptor	 *|
***		REG_3 long    ncopy)	|* # bytes to [try to] copy	 *|
**
** DESCRIPTION
**	ucopy copies ncopy bytes from file descriptor fdi to file descriptor
**	fdo.
**
**	Use ucopy to transfer data unconditionally between open files.
**
** RETURN VALUE
**	>= 0	number of bytes copied
**
**	ERROR	an error occurred.
**
** GLOBALS ACCESSED
*/

#include "ipw.h"

long
ucopy(
	int		fdi,		/* input file descriptor	 */
	int             fdo,		/* output file descriptor	 */
	REG_3 long     	ncopy)		/* # bytes to [try to] copy	 */
{
	REG_1 int       nbytes;		/* # bytes read into buffer	 */
	REG_2 long      ncopied;	/* total # bytes copied		 */

	char            buf[OPT_BUF];	/* I/O buffer			 */

	ncopied = 0;
	nbytes = 0;

 /*
  * loop until copy request is satisfied 
  */
	for (; ncopy > 0; ncopy -= nbytes) {

 /*
  * trim current read request to min(copy request, buffer size) 
  */
		if (ncopy < sizeof(buf)) {
 /* NOSTRICT */
			nbytes = ncopy;
		}
		else {
			nbytes = sizeof(buf);
		}
 /*
  * do the I/O 
  */
		nbytes = uread(fdi, buf, nbytes);
		if (nbytes <= 0) {
			if (nbytes < 0) {
				return (ERROR);
			}

			break;
		}

		if (uwrite(fdo, buf, nbytes) == ERROR) {
			return (ERROR);
		}

 /*
  * keep track of total # bytes copied 
  */
		ncopied += nbytes;
	}

	if (nbytes == ERROR) {
		return (ERROR);
	}

	return (ncopied);
}
