#include "ipw.h"
#include "pgm.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_n = {
		'n', "net radiation instead of incoming",
	};

	static OPTION_T opt_b = {
		'b', "# output bits",
		INT_OPTARGS, "nbits",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "mu, beam & diffuse irrad, mu_s, Vf, Ct, albedo",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&opt_b,
		&operands,
		0
	};

	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	int		nbits;		/* # output bits		 */


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
	no_tty(fdi);

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* process input args */

	if (got_opt(opt_b))
		nbits = int_arg(opt_b, 0);
	else
		nbits = 0;

   /* read input data, calculate, and write */

	gtoporad (fdi, fdo, got_opt(opt_n), nbits);

   /* all done */

	ipwexit(EXIT_SUCCESS);
}
