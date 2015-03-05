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
**	sunhwrite -- write an IPW SUNH header
**
** SYNOPSIS
**	#include "file.h"
**
**	int sunhwrite(fd, sunhpp)
**	int fd;
**	SUNH_T **sunhpp;
**
** DESCRIPTION
**	Sunhwrite writes the array of SUNH headers pointed to by sunhpp
**	to file descriptor fd.
**
** RESTRICTIONS
**	Sunhwrite writes the values in float precision, even though
**	internally they are stored in double precision.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**
** ERRORS
**
** WARNINGS
**
** APPLICATION USAGE
**	sunhwrite is called by IPW application programs to write
**	SUNH headers.
**
** FUTURE DIRECTIONS
**
** HISTORY
**	5/3/93	Changed ftoa calls to dtoa.  Limited precision was causing
**		problems.
**
** BUGS
*/

#include "ipw.h"
#include "hdrio.h"
#include "bih.h"
#include "sunh.h"
#include "_sunh.h"

int
sunhwrite(
	int             fd,		/* output file descriptor	 */
	SUNH_T        **sunhpp)		/* -> array of SUNH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */

	nbands = hnbands(fd);

	assert(_sunhcheck(sunhpp, nbands));

 /*
  * loop through possible output bands
  */
	for (band = 0; band < nbands; ++band) {
		REG_1 SUNH_T   *sunhp;		/* -> current SUNH	 */
		char            s[HREC_SIZ];	/* output value		 */

		sunhp = sunhpp[band];
		if (sunhp == NULL) {
			continue;
		}
 /*
  * write preamble
  */
		if (hwprmb(fd, SUNH_HNAME, band, SUNH_VERSION) == ERROR) {
			return (ERROR);
		}
 /*
  * write fields
  */
		if (hputrec(fd, (char *) NULL, SUNH_COS,
			    dtoa(s, (float) sunh_cos(sunhp)))
		    == ERROR) {
			return (ERROR);
		}
		if (hputrec(fd, (char *) NULL, SUNH_ZEN,
			    dtoa(s, (float) sunh_zen(sunhp)))
		    == ERROR) {
			return (ERROR);
		}
		if (hputrec(fd, (char *) NULL, SUNH_AZM,
			    dtoa(s, (float) sunh_azm(sunhp)))
		    == ERROR) {
			return (ERROR);
		}
	}

	return (OK);
}
