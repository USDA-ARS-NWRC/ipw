#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_w = {
		'w',
		"wavelength range in um (otherwise from stdin)",
		REAL_OPTARGS, "um",
		OPTIONAL, 1, 2
	};

	static OPTION_T opt_d = {
		'd',
		"date (year, month, day)",
		INT_OPTARGS, "d",
		OPTIONAL, 3, 3
	};

	static OPTION_T opt_a = {
		'a',
		"abbreviated output (no annotation, just numbers)"
	};

	static OPTION_T *optv[] = {
		&opt_w,
		&opt_d,
		&opt_a,
		0
	};

	double         *range;
	int            *date;

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * wavelength range or individual wavelengths
  */
	range = (got_opt(opt_w)) ? range = real_argp(opt_w) : NULL;
 /*
  * if specific date calculate radius vector, otherwise use 1.0
  */
	date = (got_opt(opt_d)) ? int_argp(opt_d) : NULL;
 /*
  * do it
  */
	solar(n_args(opt_w), range, date, got_opt(opt_a));
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
