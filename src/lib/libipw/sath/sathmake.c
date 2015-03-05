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
**	sathmake -- make an IPW SATH header
**
** SYNOPSIS
**	#include "sath.h"
**
**	SATH_T *
**	sathmake(
**		char    *platform,	|* e.g. "Landsat", "ER2", ...	 *|
**		char    *sensor,	|* e.g. "TM", "AVIRIS", ...	 *|
**		char    *location,	|* e.g. Landsat path, row, quad	 *|
**		char    *gmdate,	|* YYYYMMDD			 *|
**		char    *gmtime)	|* hhmmss.sss...		 *|
**
** DESCRIPTION
**	sathmake allocates an IPW SAT header.  The header is initialized with
**	the remaining arguments.
**
** RETURN VALUE
**	pointer to new SAT header; NULL if error
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "sath.h"

SATH_T *
sathmake(
	char           *platform,	/* e.g. "Landsat", "ER2", ...	 */
	char           *sensor,		/* e.g. "TM", "AVIRIS", ...	 */
	char           *location,	/* e.g. Landsat path, row, quad	 */
	char           *gmdate,		/* YYYYMMDD			 */
	char           *gmtime)		/* hhmmss.sss...		 */
{
	SATH_T         *sathp;		/* -> SAT header		 */

 /*
  * allocate header
  */
 /* NOSTRICT */
	sathp = (SATH_T *) hdralloc(1, sizeof(SATH_T), ERROR, SATH_HNAME);
	if (sathp == NULL) {
		return (NULL);
	}
 /*
  * initialize header
  */
	if (platform != NULL
	    && (sathp->platform = strdup(platform)) == NULL) {
		return (NULL);
	}
	if (sensor != NULL
	    && (sathp->sensor = strdup(sensor)) == NULL) {
		return (NULL);
	}
	if (location != NULL
	    && (sathp->location = strdup(location)) == NULL) {
		return (NULL);
	}
	if (gmdate != NULL
	    && (sathp->gmdate = strdup(gmdate)) == NULL) {
		return (NULL);
	}
	if (gmtime != NULL
	    && (sathp->gmtime = strdup(gmtime)) == NULL) {
		return (NULL);
	}

	return (sathp);
}
