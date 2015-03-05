#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_m = {
		'm', "mask image file",
		STR_OPERANDS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdm;		/* mask image file descriptor	 */
	int             fdo;		/* output file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access input file
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
  * access mask file
  */
	if (!got_opt(opt_m)) {
		fdm = ERROR;
	}
	else {
		fdm = uropen(str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	}

	no_tty(fdm);
 /*
  * access output file
  */
	fdo = ustdout();

	no_tty(fdo);
 /*
  * do histogram
  */
	hist(fdi, fdm, fdo);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
