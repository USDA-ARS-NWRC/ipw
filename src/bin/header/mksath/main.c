#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_p = {
		'p', "platform",
		STR_OPTARGS, "platform",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "sensor",
		STR_OPTARGS, "sensor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "location",
		STR_OPTARGS, "location",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "date (YYYYMMDD)",
		STR_OPTARGS, "date",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_t = {
		't', "time (hhmmss.sss...)",
		STR_OPTARGS, "time",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "band #s to receive header (default: all)",
		INT_OPTARGS, "band",
		OPTIONAL, 1
	};

	static OPTION_T opt_f = {
		'f', "force header output; ignore any input image",
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_p,
		&opt_s,
		&opt_l,
		&opt_d,
		&opt_t,
		&opt_b,
		&opt_f,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	*/
	int             fdo;		/* output image file descriptor	*/
	char           *gmdate;		/* date (YYYYMMDD)		*/
	char           *gmtime;		/* time (hhmmss.sss)		*/
	char           *location;	/* location			*/
	char           *platform;	/* platform			*/
	char           *sensor;		/* sensor			*/

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 5, 5, &opt_p, &opt_s, &opt_l, &opt_d, &opt_t);
 /*
  * crack options
  */
	platform = got_opt(opt_p) ? str_arg(opt_p, 0) : (char *) NULL;
	sensor = got_opt(opt_s) ? str_arg(opt_s, 0) : (char *) NULL;
	location = got_opt(opt_l) ? str_arg(opt_l, 0) : (char *) NULL;
	gmdate = got_opt(opt_d) ? str_arg(opt_d, 0) : (char *) NULL;
	gmtime = got_opt(opt_t) ? str_arg(opt_t, 0) : (char *) NULL;
 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_arg(operands, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(fdi);
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	mksath(fdi, platform, sensor, location, gmdate, gmtime,
	        n_args(opt_b), int_argp(opt_b), got_opt(opt_f), fdo);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
