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
#include <unistd.h>

#include "ipw.h"
#include "solar.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_t = {
		't', "date & time: year, month, day, hour [, min, sec]",
		INT_OPTARGS, "i",
		OPTIONAL, 4, 6
	};

	static OPTION_T opt_b = {
		'b', "latitude: deg, min, sec unless -r specified",
		REAL_OPTARGS, "lat",
		REQUIRED, 1, 3
	};

	static OPTION_T opt_l = {
		'l', "longitude: deg, min, sec unless -r specified",
		REAL_OPTARGS, "lon",
		REQUIRED, 1, 3
	};

	static OPTION_T opt_s = {
		's', "slope of surface: deg unless -r specified",
		REAL_OPTARGS, "slope",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_a = {
		'a', "azimuth of surface: deg unless -r specified",
		REAL_OPTARGS, "azm",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_r = {
		'r', "lat/lon in radians"
	};

	static OPTION_T opt_z = {
		'z', "time zone, minutes west of Greenwich (GMT if omitted)",
		INT_OPTARGS, "zone",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_y = {
		'y', "daylight savings flag"
	};

	static OPTION_T opt_d = {
		'd', "print Earth-Sun radius vector also"
	};

	static OPTION_T opt_f = {
		'f', "force printing of GMT and z even if not a tty"
	};

	static OPTION_T *optv[] = {
		&opt_t,
		&opt_b,
		&opt_l,
		&opt_s,
		&opt_a,
		&opt_r,
		&opt_z,
		&opt_y,
		&opt_d,
		&opt_f,
		0
	};

	double          azmSlope;	/* local azimuth (radians)	 */
	double          azmSun;		/* solar azimuth 		 */
	int             isDST;		/* ? Daylight Savings Time	 */
	double          latitude;	/* latitude (radians)		 */
	int             locGradient;	/* ? local slope, azimuth given  */
	double          longitude;	/* longitude (radians)		 */
	double          cosSlope;	/* cosine local slope (radians)	 */
	double          mu;		/* cosine local zenith		 */
	int             printRadVec;	/* ? print Earth-Sun radius vec  */
	double          radVec;		/* Earth-Sun radius vector	 */
	int             rtn;		/* return code			 */
	datetime_t     *tp;		/* time structure		 */
	int             zone;		/* time zone: minutes west of GMT*/

	int             year;
	int		month;
	int		day;
	int		hour;
	int		min;
	int		sec;

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION); 

 /*
  * arguments
  */
	sunang_opts(&opt_b, &opt_l, &opt_s, &opt_a, &opt_r, &latitude,
		    &longitude, &cosSlope, &azmSlope);
	zone = (got_opt(opt_z)) ? int_arg(opt_z, 0) : 0;
	locGradient = got_opt(opt_a) || got_opt(opt_s);
	isDST = got_opt(opt_y);
	printRadVec = got_opt(opt_d);

 /*
  * was a single date and time specified on command line
  */
	if (got_opt(opt_t)) {
		/*
		 *  Create date-time structure from arguments of -t option.
		 *  Convert to GMT.
		 */
		tp = make_dt(int_arg(opt_t, 0), int_arg(opt_t, 1),
			     int_arg(opt_t, 2), int_arg(opt_t, 3),
			     (n_args(opt_t) > 4) ? int_arg(opt_t, 4) : 0,
			     (n_args(opt_t) > 5) ? int_arg(opt_t, 5) : 0);
		if (tp == NULL) {
			error("Invalid date-time");
		}
		local2gmt(tp, zone, isDST);

		/*
		 *  Calculate sun angle.
		 */
		rtn = sunangle(latitude, longitude, tp, &mu, &azmSun, &radVec);
		if (rtn == ERROR) {
			error("cannot compute sun angle");
		}
		/*
		 * adjust for local gradient if specified
		 */
		if (locGradient) {
			if (rotate(mu, azmSun, cosSlope, azmSlope, &mu, &azmSun)
				== ERROR) {
				error("cannot adjust for local gradient");
			}
		}

		/*
		 *  Print results.
		 */
		if (isatty(STDOUT_FILENO) || got_opt(opt_f) ) {
			(void) printf("\nGMT %s", dt2str(tp));
			if (rtn == 1) {
				(void) printf("(below horizon) ");
			}
			(void) printf("-z %.5g ", RAD_TO_DEG(acos(mu)));
		}
		(void) printf("-u %f -a %.5g", mu, RAD_TO_DEG(azmSun));
		if (printRadVec) {
			(void) printf(" -d %.5g", radVec);
		}
		printf("\n");
	}

 /*
  * no date and time on command line, so read dates and times from standard
  * input
  */
	else {
		while(scanf("%d ,%d ,%d ,%d ,%d ,%d", &year, &month, &day,
			    &hour, &min, &sec) == 6) {
			/*
			 *  Create date-time structure, and convert to GMT.
			 */
			tp = make_dt(year, month, day, hour, min, sec);
			if (tp == NULL) {
				error("Invalid date-time");
			}
			local2gmt(tp, zone, isDST);

			/*
			 *  Calculate sun angle.
			 */
			rtn = sunangle(latitude, longitude, tp, &mu, &azmSun,
					&radVec);
			if (rtn == ERROR) {
				error("cannot compute sun angle");
			}

			/*
			 * adjust for local gradient if specified
			 */
			if (locGradient) {
				if (rotate(mu, azmSun, cosSlope, azmSlope,
					   &mu, &azmSun) == ERROR) {
				      error("cannot adjust for local gradient");
				}
			}

			/*
			 *  Print results.
			 */
			(void) printf("%.5g %f %.5g", RAD_TO_DEG(acos(mu)),
					mu, RAD_TO_DEG(azmSun));
			if (printRadVec) {
				(void) printf(" %.5g", radVec);
			}
			printf("\n");
		}
	}

	ipwexit(EXIT_SUCCESS);
}
