#include "ipw.h"
#include "pgm.h"

int
main (
	int		argc,
	char           *argv[])
{
	static OPTION_T opt_i = {
		'i', "invert potential temperature (default is Ta)"
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_e = {
		'e', "input elevation image (m)",
		STR_OPTARGS, "elev_image",
		REQUIRED, 1, 1,
	};

	static OPTION_T operand = {
		OPERAND, "input temperature image",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_l = {
		'l', "lapse rate (def: -6.5 deg C/km)",
		REAL_OPTARGS, "val",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_s = {
		's', "sea-level temp. (def: 27 deg C)",
		REAL_OPTARGS, "val",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_L = {
		'L', "do simple linear adjustment based on lapse rate & elev"
	};

	static OPTION_T *optv[] = {
		&opt_i,
		&opt_m,
		&opt_e,
		&opt_l,
		&opt_s,
		&opt_L,
		&operand,
		0
	};

	int		fdi;		/* input image file descriptor	*/
	int		fdm;		/* mask image file descriptor	*/
	int		fdo;		/* output image file descriptor	*/
	int		fd_elev;	/* elev image file descriptor	*/
	int		inv;		/* Invert potential temp flag   */
	float		lapse;		/* lapse rate 			*/
	float		T0;		/* assumed sea-level temp.	*/
	int		linear;		/* linear adjustment flag	*/

   /* get options */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* check inversion and linear options */

	inv    = got_opt (opt_i);
	linear = got_opt (opt_L);

   /* access numerical options */

	if (got_opt (opt_l)) {
	    lapse = real_arg (opt_l, 0);
	} else {
	    lapse = -6.5;
	}

	if (got_opt (opt_s)) {
	    T0 = real_arg (opt_s, 0);
	} else {
	    T0 = 26.84;
	}

   /* access operand */

	if (got_opt (operand)) {
		fdi = uropen (str_arg(operand, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operand, 0));
		}
	} else {
		fdi = ustdin();
	}

   /* access elevation image */

	fd_elev = uropen (str_arg(opt_e, 0));
	if (fd_elev == ERROR) {
		error("can't open \"%s\"", str_arg(opt_e, 0));
	}

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

   /* access output image */

	fdo = ustdout();

   /* can't read from or write to a tty */

	no_tty (fdi);
	no_tty (fdo);

   /* read/write headers */

	headers (fdi, fdm, fd_elev, inv, fdo);

   /* compute temps/write output image */

	ipott (fdi, fdm, fd_elev, inv, fdo, lapse, T0, linear);

   /* finished */

	ipwexit(EXIT_SUCCESS);
}
