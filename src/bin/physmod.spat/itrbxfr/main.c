#include	"ipw.h"
#include	"pgm.h"

int
main (
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "upper height (meters)",
		REAL_OPTARGS, "upper_height",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "delta time (hrs)",
		REAL_OPTARGS, "delta_t",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input image (defaults to stdin)",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_d,
		&opt_z,
		&opt_m,
		&operand,
		0
	};

	int	fdi;		/* input image file descriptor	*/
	int	fdm;		/* mask image file descriptor	*/
	int	fdo;		/* output image file descriptor */
	double	delta_t;	/* delta time (s)		*/
	double	z;		/* upper height (m)		*/

   /* get args */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

   /* process args */

	delta_t = real_arg (opt_d, 0);
	delta_t = HR_TO_SEC(delta_t);

	if (got_opt (opt_z)) {
		z = real_arg (opt_z, 0);
	} else {
		z = Z_DEFAULT;
	}

   /* access mask image if specified */

	if (got_opt (opt_m)) {
		fdm = uropen (str_opnd(opt_m, 0));
		if (fdm == ERROR)
			error ("Can't open \"%s\"", str_opnd(opt_m, 0));
	} else {
		fdm = ERROR;
	}
  
   /* access input image */

	if (got_opt (operand)) {
		fdi = uropen (str_opnd(operand, 0));
		if (fdi == ERROR)
			error ("Can't open \"%s\"", str_opnd(operand, 0));
	} else {
		fdi = ustdin();
	}

    /* can't read or write tty */

	no_tty (fdi);
	fdo = ustdout ();
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdm, fdo, IBANDS, OBANDS);

   /* read input image, perform calculations and write output image */

	itrbxfr (fdi, fdm, fdo, delta_t, z);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
