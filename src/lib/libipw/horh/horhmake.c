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
**	horhmake -- make an IPW HORH header
**
** SYNOPSIS
**	#include "horh.h"
**
**	HORH_T *
**	horhmake(
**		double          azm)	|* direction to horizon		 *|
**
** DESCRIPTION
**	Horhmake allocates a horizon header.  The header is initialized
**	with the azimuth argument.
**
** RETURN VALUE
**	pointer to new HORH header; NULL if error
*/

#include <math.h>

#include "ipw.h"
#include "hdrio.h"
#include "horh.h"
#include "_horh.h"

HORH_T *
horhmake(
	double          azm)		/* direction to horizon		 */
{
	HORH_T         *horhp;		/* -> HORH array		 */
	double          pi;

	pi = 4 * atan(1.);

	if (fabs(azm) > pi) {
		if (fabs(azm) < pi + TOL) {
			if (azm > 0)
				azm = pi;
			else
				azm = -pi;
		}
		else {
			usrerr("horhmake: abs(azm) (%g) > pi", azm);
			return (NULL);
		}
	}
 /*
  * allocate header
  */
 /* NOSTRICT */
	horhp = (HORH_T *) hdralloc(1, sizeof(HORH_T), ERROR, HORH_HNAME);
	if (horhp == NULL) {
		return (NULL);
	}

	horhp->azimuth = azm;

	return (horhp);
}
