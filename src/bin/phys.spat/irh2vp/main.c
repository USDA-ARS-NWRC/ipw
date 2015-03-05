#include "ipw.h"
#include "pgm.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "inverse operation"
	};

	static OPTION_T opt_c = {
		'c', "clip RH to <=1.0"
	};

	static OPTION_T opt_b = {
		'b', "number bits in output image",
		INT_OPTARGS, "#bits",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_c,
		&opt_b,
		&opt_m,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdm;		/* mask image file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	bool_t		inverse;	/* flag for inverse operation	 */
	bool_t		clip;		/* flag for clipping RH to <=1.0 */
	int		nbits;		/* # bits in output image	 */


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

   /* process options */

	inverse = got_opt (opt_i);

	clip = got_opt (opt_c);

	if (got_opt (opt_b))
		nbits = int_arg (opt_b, 0);
	else
		nbits = 0;

   /* access mask file */

	if (!got_opt(opt_m)) {
		fdm = ERROR;
	} else {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error ("can't open \"%s\"", str_arg(opt_m, 0));
		}
		no_tty(fdm);
	}


   /* access output file */

	fdo = ustdout();
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdm, nbits, fdo);

   /* do it */

	irh2vp (fdi, fdm, inverse, clip, fdo);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
