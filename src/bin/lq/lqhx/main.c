
#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_h = {
		'h', "new LQ header",
		STR_OPTARGS, "LQH",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "input image file",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_h,
		&opt_i,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access input LQH
  */
	if (!got_opt(opt_h)) {
		parm.h_fd = ustdin();
	}
	else {
		parm.h_fd = uropen(str_arg(opt_h, 0));
		if (parm.h_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_h, 0));
		}
	}

	no_tty(parm.h_fd);
 /*
  * access input image
  */
	if (!got_opt(opt_i)) {
		if (!got_opt(opt_h)) {
			error("either -h or -i must be specified");
		}
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(opt_i, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * do it
  */
	headers();
	lqhx();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
