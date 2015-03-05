#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "input image file",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "lookup table file",
		STR_OPTARGS, "LUT",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_l,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdl;		/* LUT file descriptor		 */
	int             fdo;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 2, 2, &opt_i, &opt_l);
 /*
  * access input image
  */
	if (got_opt(opt_i)) {
		fdi = uropen(str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}
	else {
		fdi = ustdin();
	}

	no_tty(fdi);
 /*
  * access LUT
  */
	if (got_opt(opt_l)) {
		fdl = uropen(str_arg(opt_l, 0));
		if (fdl == ERROR) {
			error("can't open \"%s\"", str_arg(opt_l, 0));
		}
	}
	else {
		fdl = ustdin();
	}

	no_tty(fdl);
 /*
  * access output file
  */
	fdo = ustdout();
	no_tty(fdo);
 /*
  * do it
  */
	lutx(fdi, fdl, fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
