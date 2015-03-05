#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_c = {
		'c', "class image file",
		STR_OPERANDS, "image",
		REQUIRED, 1, 1,
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_c,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * process options
  *
  * Although the code below treats the -c option as optional, it is now
  * required (see the definition of "opt_c" above).  The program crashes
  * when the -c option is not specified; the routines "accum" assume
  * certain data structures are initialized, and they ain't when there's
  * no class file.  No one seems to said anything about this bug, so
  * I made opt_c REQUIRED.
  * J. Domingo, OSU, 23 Oct 1997
  */
	if (!got_opt(opt_c)) {
		parm.c_fd = ERROR;
	}
	else {
		parm.c_fd = uropen(str_arg(opt_c, 0));
		if (parm.c_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}

		no_tty(parm.c_fd);
	}

 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operands, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * do it
  */
	headers();
	mstats();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
