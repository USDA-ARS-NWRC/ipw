/*
** parse command line arguments
*/

#include <math.h>
#include <time.h>

#include "ipw.h"

#include "pgm.h"

void
sunlight_opts(
	OPTION_T       *d,		/* date			 */
	OPTION_T       *b,		/* latitude		 */
	OPTION_T       *l,		/* longitude		 */
	OPTION_T       *r,		/* ? radians		 */
	OPTION_T       *z,		/* zone			 */
	OPTION_T       *y,		/* ? daylight savings	 */
	int            *zone,		/* min W of Greenwich	 */
	bool_t         *isdst,		/* ? DST flag		 */
	double         *lat,		/* latitude		 */
	double         *lon,		/* longitude		 */
	int            *year,		/* full year		 */
	int            *month,		/* month (1-12)		 */
	int            *day)		/* day (1-31)		 */
{
	double          pio4;

	pio4 = atan(1.);
 /*
  * latitude
  */
 /* radians	 */
	if (got_opt(*r)) {
		*lat = real_arg(*b, 0);
	}
 /* degrees	 */
	else {
		*lat = fabs(real_arg(*b, 0));
		if (n_args(*b) > 1)
			*lat += real_arg(*b, 1) / MIN_DEG;
		if (n_args(*b) > 2)
			*lat += real_arg(*b, 2) / SEC_DEG;
		if (real_arg(*b, 0) < 0)
			*lat = -(*lat);
		*lat *= pio4 / 45;
	}

 /*
  * longitude
  */
 /* radians      */
	if (got_opt(*r)) {
		*lon = real_arg(*l, 0);
	}
 /* degrees      */
	else {
		*lon = fabs(real_arg(*l, 0));
		if (n_args(*l) > 1)
			*lon += real_arg(*l, 1) / MIN_DEG;
		if (n_args(*l) > 2)
			*lon += real_arg(*l, 2) / SEC_DEG;
		if (real_arg(*l, 0) < 0)
			*lon = -(*lon);
		*lon *= pio4 / 45;
	}
 /*
  * daylight savings?
  */
	*isdst = got_opt(*y);
 /*
  * date
  */
	*year = int_arg(*d, 0);
	*month = int_arg(*d, 1);
	*day = int_arg(*d, 2);
 /*
  * time zone
  */
	*zone = (got_opt(*z)) ? int_arg(*z, 0) : 0;
}
