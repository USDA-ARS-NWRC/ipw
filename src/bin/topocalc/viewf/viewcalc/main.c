#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_s = {
		's', "input slope/aspect image (output from gradient)",
		STR_OPTARGS, "slope/aspect",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "input multi-angle horizon image",
		STR_OPTARGS, "horizons",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_s,
		&opt_h,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input files
  */
	if (!got_opt(opt_s)) {
		parm.i_fds = ustdin();
	}
	else {
		parm.i_fds = uropen(str_arg(opt_s, 0));
		if (parm.i_fds == ERROR) {
			error("can't open \"%s\"", str_arg(opt_s, 0));
		}
	}
	if (!got_opt(opt_h)) {
		if (!got_opt(opt_s))
			error("must specify at least one of -s -h");
		parm.i_fdh = ustdin();
	}
	else {
		parm.i_fdh = uropen(str_arg(opt_h, 0));
		if (parm.i_fdh == ERROR) {
			error("can't open \"%s\"", str_arg(opt_h, 0));
		}
	}

	no_tty(parm.i_fds);
	no_tty(parm.i_fdh);
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * process headers
  */
	headers();
 /*
  * allocate buffers
  */
	buffers();
 /*
  * values in trig tables
  */
	trigtbl();
 /*
  * read input data and calculate view factors, and write
  */
	viewf();
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
