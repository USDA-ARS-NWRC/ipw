#include <unistd.h>

#include "ipw.h"
#include "pgm.h"

#define DEFAULT_FMT_STR "%Y_%m_%d+%H:%M:%S"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_d = {
		'd', "date-time as year,month,day,hour,min,sec",
		INT_OPTARGS, "i",
		OPTIONAL, 6, 6
	};

	static OPTION_T opt_h = {
		'h', "hours in time interval",
		REAL_OPTARGS, "hours",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "minutes in time interval",
		REAL_OPTARGS, "minutes",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "seconds in time interval",
		INT_OPTARGS, "seconds",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_v = {
		'v', "verbose -- print dates in english while translating"
	};

	static OPTION_T operand = {
		OPERAND, "date-time to add time interval to",
		STR_OPERANDS, "date-time",
		OPTIONAL, 1, 1
	};
	static OPTION_T *optv[] = {
		&opt_d,
		&opt_h,
		&opt_m,
		&opt_s,
		&opt_v,
		&operand,
		0
	};

	datetime_t     *dt;		/* date-time if -d option used	 */
	datetime_t      dt_opnd;	/* date-time if given as operand */
	bool_t		verbose;
	int		interval;	/* total time interval (seconds) */
	float		interval_hrs;	/* hours in interval (-h option) */
	float		interval_mins;	/* mins in interval (-m option)  */
	int		interval_secs;	/* secs in interval (-s option)  */
	char		buf[MAX_INPUT];	/* for printing formatted dt	 */


	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 *  Make sure we have at least one of: -h, -m, -s
	 */
	opt_check(1, 3, 3, &opt_h, &opt_m, &opt_s);

	/*
	 *  Get the date-time from -d option or operand.
	 */
	if (got_opt(opt_d) && got_opnds(operand)) {
		error("date-time given with both -d option and as operand");
	}
	else if (got_opt(opt_d)) {
		dt = make_dt(int_arg(opt_d, 0), int_arg(opt_d, 1),
			     int_arg(opt_d, 2), int_arg(opt_d, 3),
			     int_arg(opt_d, 4), int_arg(opt_d, 5));
		if (dt == NULL) {
			error ("Invalid date-time");
		}
	}
	else if (got_opnds(operand)) {
		dt = &dt_opnd;
		if (! str2dt(str_opnd(operand, 0), DEFAULT_FMT_STR, dt)) {
			error ("Invalid date-time");
		}
	}
	else {
		error("No date-time given with -d option or as operand");
	}

	/*
	 *  Print date-time if verbose mode.
	 */
	verbose = got_opt(opt_v);
	if (verbose) {
		fprintf(stderr, "%s", dt2str(dt));
	}

	/*
	 *  Compute the total length of time interval, and add it to date-time.
	 */
	interval = 0;
	if (got_opt(opt_h)) {
		interval_hrs = real_arg(opt_h, 0);
		interval = HR_TO_SEC(interval_hrs);
		if (verbose)
			fprintf(stderr, "  + %f hours", interval_hrs);
	}
	if (got_opt(opt_m)) {
		interval_mins = real_arg(opt_m, 0);
		interval += MIN_TO_SEC(interval_mins);
		if (verbose)
			fprintf(stderr, "  + %f minutes", interval_mins);
	}
	if (got_opt(opt_s)) {
		interval_secs = int_arg(opt_s, 0);
		interval += interval_secs;
		if (verbose)
			fprintf(stderr, "  + %d seconds", interval_secs);
	}
	add_to_dt(dt, 0, interval);

	if (verbose) {
		fprintf(stderr, "\n===> %s", dt2str(dt));
	}

	/*
	 *  Print the resulting date-time
	 */
	if (got_opt(opt_d)) {
		printf("%d,%d,%d,%d,%d,%d\n", dt->year, dt->month, dt->day,
			dt->hour, dt->min, dt->sec);
	} else {
		if (dt2fmtstr(dt, DEFAULT_FMT_STR, buf)) {
			printf("%s\n", buf);
		} else {
			error("Cannot format date-time for output");
		}
	}

	ipwexit (EXIT_SUCCESS);
}
