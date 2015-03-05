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
**	geohmake -- make an IPW GEOH header
**
** SYNOPSIS
**	#include "geoh.h"
**
**	GEOH_T *
**	geohmake(
**		double     bline,	|* begin line #			 *|
**		double     bsamp,	|* begin sample #		 *|
**		double     dline,	|* line increment		 *|
**		double     dsamp,	|* sample increment		 *|
**		char      *units,	|* units of measurement		 *|
**		char      *csys)	|* coord system ID		 *|
**
** DESCRIPTION
**	geohmake allocates a GEO header.  The header is initialized with the
**	remaining arguments.
**
** RETURN VALUE
**	pointer to new GEO header; NULL if error
*/

#include <string.h>

#include "ipw.h"
#include "geoh.h"
#include "hdrio.h"

GEOH_T *
geohmake(
	double          bline,		/* begin line #			 */
	double          bsamp,		/* begin sample #		 */
	double          dline,		/* line increment		 */
	double          dsamp,		/* sample increment		 */
	char           *units,		/* units of measurement		 */
	char           *csys)		/* coord system ID		 */
{
	GEOH_T         *geohp;		/* -> GEOH array		 */

	assert(dline != 0.0);
	assert(dsamp != 0.0);
 /*
  * allocate header
  */
 /* NOSTRICT */
	geohp = (GEOH_T *) hdralloc(1, sizeof(GEOH_T), ERROR, GEOH_HNAME);
	if (geohp == NULL) {
		return (NULL);
	}

	geohp->bline = bline;
	geohp->bsamp = bsamp;
	geohp->dline = dline;
	geohp->dsamp = dsamp;

	if (units != NULL
	    && (geohp->units = strdup(units)) == NULL) {
		usrerr("insufficient memory");
		return (NULL);
	}
	if (csys != NULL
	    && (geohp->csys = strdup(csys)) == NULL) {
		usrerr("insufficient memory");
		return (NULL);
	}

	return (geohp);
}
