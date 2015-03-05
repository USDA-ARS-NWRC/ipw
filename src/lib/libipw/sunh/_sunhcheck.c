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
**	sunhcheck -- validate components of IPW SUNH header
**
** SYNOPSIS
**	#include "_sunh.h"
**
**	bool_t
**	_sunhcheck(
**		SUNH_T   **sunhpp,	|* -> array of -> SUNH header	 *|
**		int        nbands)	|* # header bands		 *|
**
** DESCRIPTION
**	_sunhcheck checks that sunhpp points to an array of nbands
**	pointers to valid SUNH headers.
**
** RETURN VALUE
**	TRUE if sunhpp checks OK, else FALSE.
*/

#define TOL	(16 * FLT_EPSILON)

#include <math.h>

#include "ipw.h"
#include "sunh.h"
#include "_sunh.h"

bool_t
_sunhcheck(
	SUNH_T        **sunhpp,		/* -> array of -> SUNH header	 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	bool_t          found;		/* ? found at least 1 header	 */
	double          cosz;


	assert(sunhpp != NULL);
 /*
  * loop through possible bands
  */
	found = FALSE;

	for (band = 0; band < nbands; ++band) {
		SUNH_T         *sunhp;	/* -> SUNH for current band	 */

		sunhp = sunhpp[band];
		if (sunhp == NULL) {
			continue;
		}

		found = TRUE;
 /*
  * make sure azimuth <= M_PI, within tolerance TOL
  */
		if (fabs(sunh_azm(sunhp)) > M_PI) {
			if (fabs(sunh_azm(sunhp)) > M_PI + TOL) {
				usrerr("\"%s\" header, band %d: %g: bad azm",
				   SUNH_HNAME, band, sunh_azm(sunhp));
				return (FALSE);
			}
			else {
				sunhp->azm_sun = (sunh_azm(sunhp) > M_PI) ?
					M_PI : -M_PI;
			}
		}
 /*
  * make sure cosine within range
  */
		if (fabs(sunh_cos(sunhp)) > 1) {
			usrerr("\"%s\" header, band %d: %g: bad cos",
			       SUNH_HNAME, band, sunh_cos(sunhp));
			return (FALSE);
		}
 /*
  * make sure zenith and its cosine match
  */
		if ((cosz = cos(sunh_zen(sunhp))) != sunh_cos(sunhp)) {
			if (fabs(cosz - sunh_cos(sunhp)) < TOL) {
				sunhp->zen_sun = acos(sunh_cos(sunhp));
			}
			else {
				usrerr("\"%s\" header, band %d: %g %g: bad zen cos",
				    SUNH_HNAME, band, sunh_zen(sunhp),
				       sunh_cos(sunhp));
				return (FALSE);
			}
		}
	}

	assert(found);

	return (TRUE);
}
