#include "ipw.h"

#include "pgm.h"

int
main(
		int             argc,
		char          **argv)
{
	int i;

	static OPTION_T opt_l = {
			'l', "return # lines"
	};
	static OPTION_T opt_s = {
			's', "return # samps"
	};
	static OPTION_T opt_b = {
			'b', "return # bands"
	};
	static OPTION_T opt_y = {
			'y', "return # bytes/pixel"
	};
	static OPTION_T opt_f = {
			'f', "return filename"
	};
	static OPTION_T opt_m = {
			'm', "strip labels for easier machine readable output"
	};

	static OPTION_T operands = {
			OPERAND, "input image file",
			STR_OPERANDS, "image",
			OPTIONAL, 1, 0,
	};

	static OPTION_T *optv[] = {
			&opt_l,
			&opt_s,
			&opt_b,
			&opt_y,
			&opt_f,
			&opt_m,
			&operands,
			0
	};

	/*
	 * begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 * set any requested options
	 */
	if ( got_opt(opt_l) ) SETBIT(DOLINES);
	if ( got_opt(opt_s) ) SETBIT(DOSAMPS);
	if ( got_opt(opt_b) ) SETBIT(DOBANDS);
	if ( got_opt(opt_y) ) SETBIT(DOBYTES);
	if ( got_opt(opt_f) ) SETBIT(DOFILE);
	/*
	 * if nothing's set, set everything
	 */
	if ( parm.flags == 0 ) parm.flags = ~0;

	/*
	 * set machine read flag
	 */
	if ( got_opt(opt_m) ) parm.machread = TRUE;

	/*
	 * access input file(s)
	 */
	if (!got_opt(operands)) {
		parm.i_fd = ustdin();
		no_tty(parm.i_fd);
		headers();
	}
	else {
		for(i = 0 ; i < n_args(operands) ; ++i) {
			parm.i_fd = uropen(str_arg(operands, i));
			if (parm.i_fd == ERROR) {
				error("can't open \"%s\"",
						str_arg(operands, i));
			}
			no_tty(parm.i_fd);
			headers();
			uclose (parm.i_fd);
		}
	}

	/*
	 * end
	 */
	ipwexit(EXIT_SUCCESS);
}
