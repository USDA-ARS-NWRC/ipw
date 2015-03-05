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

#include <math.h>

#include "ipw.h"
#include "solar.h"

int
sunangle(
	double      latitude,		/* radians, southern lats < 0	 */
	double      longitude,		/* radians, western hemi. < 0	 */
	datetime_t *datetime,		/* date and time (GMT)		 */

     /* output parameters */

	double *    mu,			/* cosine of solar zenith angle	 */
	double *    azmSun,		/* solar azimuth angle, radians
					     (west of south < 0)	 */
	double *    radVec)		/* Earth-Sun radius vector	 */
{
	double	    	rv;		/* local Earth-Sun radius vector */
	double          declination;	/* solar declination		 */
	double          omega;		/* solar longtiude		 */

	if (radVec == NULL) {
		radVec = &rv;
	}

	/*
	 * calculate ephemeris parameters
	 */
	if (ephemeris(datetime, radVec, &declination, &omega) == ERROR) {
		return (ERROR);
	}

	/*
	 * calculate sun angle
	 */
	return sunpath(latitude, longitude, declination, omega, mu, azmSun);
}
