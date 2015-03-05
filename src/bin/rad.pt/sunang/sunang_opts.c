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
** parse command line arguments
*/

#include <math.h>

#include "ipw.h"
#include "pgm.h"

void
sunang_opts(
   	OPTION_T       *b,		/* latitude		 */
	OPTION_T       *l,		/* longitude		 */
	OPTION_T       *s,		/* local slope		 */
	OPTION_T       *a,		/* local azimuth	 */
	OPTION_T       *r,		/* ? radians		 */
	double         *lat,		/* latitude		 */
	double         *lon,		/* longitude		 */
	double         *slope,		/* cosine of local slope */
	double         *azm)		/* local azimuth	 */
{
 /*
  * latitude: if radians just get the number from command line;
  * otherwise parse degrees, minutes, and seconds. The minutes and
  * seconds are optional; degrees, minutes, or seconds are not
  * restricted to integers. Latitudes in the southern hemisphere are
  * negative.
  */
	if (got_opt(*r)) {		/* radians	 */
		*lat = real_arg(*b, 0);
	}
	else {				/* degrees	 */
		*lat = fabs(real_arg(*b, 0));
		if (n_args(*b) > 1)	/* minutes	 */
			*lat += MIN_TO_DEG(real_arg(*b, 1));
		if (n_args(*b) > 2)	/* seconds	 */
			*lat += SEC_TO_DEG(real_arg(*b, 2));
		if (real_arg(*b, 0) < 0)/* southern hem	 */
			*lat = -(*lat);
		if (fabs(*lat) > 90) {
			error("latitude (%g) must be between +/- 90",
			      *lat);
		}
		*lat = DEG_TO_RAD(*lat);	/* convert to radians	 */
	}

 /*
  * longitude: parse just like latitude above. Longitudes in the
  * western hemisphere are negative.
  */
	if (got_opt(*r)) {		/* radians      */
		*lon = real_arg(*l, 0);
	}
	else {				/* degrees      */
		*lon = fabs(real_arg(*l, 0));
		if (n_args(*l) > 1)	/* minutes	 */
			*lon += MIN_TO_DEG(real_arg(*l, 1));
		if (n_args(*l) > 2)	/* seconds	 */
			*lon += SEC_TO_DEG(real_arg(*l, 2));
		if (real_arg(*l, 0) < 0)/* western hem	 */
			*lon = -(*lon);
		if (fabs(*lon) > 180) {
			error("longitude (%g) must be between +/- 180",
			      *lon);
		}
		*lon = DEG_TO_RAD(*lon);	/* convert to radians	 */
	}

 /*
  * local slope and azimuth: either radians or decimal degrees
  */
	if (got_opt(*s)) {
		*slope = real_arg(*s, 0);
		if (!got_opt(*r)) {
			*slope = DEG_TO_RAD(*slope);    /* convert to radians */
		}
	}
	else {
		*slope = 0;
	}

 /*
  * calculate cosine of slope
  */
	*slope = cos(*slope);

	if (got_opt(*a)) {
		*azm = real_arg(*a, 0);
		if (!got_opt(*r)) {
			*azm = DEG_TO_RAD(*azm);	/* convert to radians */
		}
	}
	else {
		*azm = 0;
	}
}
