#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_l = {
		'l', "line zoom factor",
		INT_OPTARGS, "factor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "sample zoom factor",
		INT_OPTARGS, "factor",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_f = {
		'f', "force coincident images",
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_s,
		&opt_f,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * check options
  */
	opt_check(1, 2, 2, &opt_l, &opt_s);

	if (!got_opt(opt_l)) {
		parm.skip_lines = 1;
		parm.dup_lines = 1;
	}
	else {
		int             z_lines;/* line zoom factor	 */

		z_lines = int_arg(opt_l, 0);
		if (z_lines == 0) {
			error("line zoom factor must be non-zero");
		}

		parm.skip_lines = z_lines < 0 ? -z_lines : 1;
		parm.dup_lines = z_lines < 0 ? 1 : z_lines;
	}

	if (!got_opt(opt_s)) {
		parm.skip_samps = 1;
		parm.dup_samps = 1;
	}
	else {
		int             z_samps;/* sample zoom factor	 */

		z_samps = int_arg(opt_s, 0);
		if (z_samps == 0) {
			error("sample zoom factor must be non-zero");
		}

		parm.skip_samps = z_samps < 0 ? -z_samps : 1;
		parm.dup_samps = z_samps < 0 ? 1 : z_samps;
	}
	
	if (got_opt(opt_f)) {
		parm.force = 1;
		if (parm.skip_samps != 1 || parm.skip_lines != 1) {
		    warn("-f option not valid when subsampling.  Ignored.");
		    parm.force = 0;
		}
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
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * do it
  */
	headers();
	zoom();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
