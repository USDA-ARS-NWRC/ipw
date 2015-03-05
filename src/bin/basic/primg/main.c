#include "ipw.h"
#include "pgm.h"
PARM_T    parm;          /* global structure */

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_a = {
		'a', "print all pixels",
	};

	static OPTION_T opt_r = {
		'r', "print raw values (i.e., don't convert to floating-point)",
	};

	static OPTION_T opt_c = {
		'c', "input coordinate file (default: stdin)",
		STR_OPTARGS, "coordfile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "input image file (default: stdin)",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_g = {
		'g', "use geodetic coord system from band {band} of input img",
		INT_OPTARGS, "band",
		OPTIONAL, 1, 1
	};
	static OPTION_T *optv[] = {
		&opt_a,
		&opt_r,
		&opt_c,
		&opt_i,
		&opt_g,
		0
	};

	int             fdc;		/* coordinate file descriptor	 */
	int             fdi;		/* input image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 2, 3, &opt_i, &opt_c, &opt_a);
	opt_check(0, 1, 2, &opt_c, &opt_a);
 /*
  * access input file(s)
  */
	if (!got_opt(opt_i)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}

	no_tty(fdi);

	if (got_opt(opt_a)) {
		fdc = ERROR;
	}
	else if (!got_opt(opt_c)) {
		fdc = ustdin();
	}
	else {
		fdc = uropen(str_arg(opt_c, 0));
		if (fdc == ERROR) {
			error("can't open \"%s\"", str_arg(opt_c, 0));
		}
	}

	if (got_opt(opt_g)) {
                parm.geoband = int_arg(opt_g, 0);
	}
	else parm.geoband = -1;
 
 /*
  * do it
  */
	primg(fdi, fdc, got_opt(opt_r));
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
