#include        <math.h>
#include	"ipw.h"
#include        "bih.h"
#include        "pixio.h"
#include        "fpio.h"
#include        "envphys.h"
#include	"pgm.h"

int
main (
	int	argc,
	char	**argv)
{
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
		&opt_m,
		&operand,
		0
	};

	int	fdi;		/* input image file descriptor	*/
	int	fdm;		/* mask image file descriptor	*/
	int	fdo;		/* output image file descriptor */


   /* get args */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

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

	idewpt (fdi, fdm, fdo);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
