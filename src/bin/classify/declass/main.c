#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_h = {
		'h', "linear quantization header",
		STR_OPERANDS, "lqh",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "#bits in output image for each band",
		INT_OPERANDS, "#bits,...",
		OPTIONAL, 1,
	};

	static OPTION_T operands = {
		OPERAND, "input image file (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_h,
		&opt_b,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int		fdh;		/* lqh file descriptor	         */
	int             fdo;		/* output file descriptor	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (!got_opt(operands)) {
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(operands, 0));
		if (fdi == ERROR) {
			error ("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty (fdi);

   /* access input LQH */

	if (got_opt(opt_h)) {

		fdh = uropen (str_arg(opt_h, 0));
		if (fdh == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_h, 0));
		}
	} else {
		fdh = ERROR;
	}

   /* access output file */

	fdo = ustdout();
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdh, int_argp(opt_b), n_args(opt_b), fdo);

   /* do declassification */

	declass (fdi, fdo);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
