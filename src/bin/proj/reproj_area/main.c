#include "ipw.h"
#include "pgm.h"

int
main (
	int             argc,
	char          **argv)
{
	static OPTION_T opt_i = {
		'i', "projection definition file for mask image",
		STR_OPTARGS, "inproj",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "projection definition file for output projection",
		STR_OPTARGS, "outproj",
		REQUIRED, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input mask image (defaults to stdin)",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_o,
		&operand,
		0
	};

	int		fdi;		/* input image file descriptor	 */
	double		bline;		/* UL input line coordinate	 */
	double		bsamp;		/* UL input sample coordinate	 */
	double		dline;		/* input pixel line increment	 */
	double		dsamp;		/* input pixel sample increment  */
	struct projdef *iproj;		/* input image proj parameters	 */
	struct projdef *oproj;		/* output image proj parameters	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input image */

	if (got_opt(operand)) {
		fdi = uropen (str_arg(operand, 0));
		if (fdi == ERROR) {
			error ("error opening file %s", str_arg(operand, 0));
		}
	} else {
		fdi = ustdin();
	}
	no_tty (fdi);

   /* read projection definition files */

	iproj = read_proj (str_arg (opt_i, 0));
	if (iproj == NULL) {
		error ("error reading file %s", str_arg (opt_i, 0));
	}

	oproj = read_proj (str_arg (opt_o, 0));
	if (oproj == NULL) {
		error ("error reading file %s", str_arg (opt_o, 0));
	}

   /* read input images headers */

	headers (fdi, &bline, &bsamp, &dline, &dsamp);

   /* reproject to calculate minimum output area*/

	reproj_area (fdi, bline, bsamp, dline, dsamp, iproj, oproj);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
