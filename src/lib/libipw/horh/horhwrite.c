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
**	horhwrite -- write an IPW HORH header
**
** SYNOPSIS
**	#include "file.h"
**
**	int
**	horhwrite(
**		int             fd,	|* output file descriptor	 *|
**		HORH_T        **horhpp)	|* -> array of HORH pointers	 *|
**
** DESCRIPTION
**      horhwrite writes the array of HORH headers pointed to by horhpp
**	to file descriptor fd.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "horh.h"
#include "_horh.h"

int
horhwrite(
	int             fd,		/* output file descriptor	 */
	HORH_T        **horhpp)		/* -> array of HORH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */

	nbands = hnbands(fd);

	assert(_horhcheck(horhpp, nbands));

 /*
  * loop through possible output bands
  */
	for (band = 0; band < nbands; ++band) {
		REG_1 HORH_T   *horhp;	/* -> current HORH		 */
		char            s[HREC_SIZ];	/* output value		 */

		horhp = horhpp[band];
		if (horhp == NULL) {
			continue;
		}
 /*
  * write preamble
  */
		if (hwprmb(fd, HORH_HNAME, band, HORH_VERSION) == ERROR) {
			return (ERROR);
		}
 /*
  * write fields
  */
		if (hputrec(fd, (char *) NULL, HORH_AZM,
			    dtoa(s, horhp->azimuth)) == ERROR) {
			return (ERROR);
		}
	}

	return (OK);
}
