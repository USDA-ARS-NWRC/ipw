#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPTARGS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "solar azimuth image",
		STR_OPTARGS, "solar_image",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_z = {
		'z', "flag for solar zenith angle in solar_image",
	};

	static OPTION_T opt_i = {
		'i', "input slope/azimuth file",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_m,
		&opt_s,
		&opt_z,
		&opt_i,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int		fdm;		/* mask image file descriptor	 */
	int             fds;		/* solar image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */

	bool_t		zenith;		/* flag set if zenith in solar file	*/


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access input file */

	if (!got_opt(opt_i)) {
		if (!got_opt(opt_s)) {
			fprintf (stderr, "ERROR: at least one of (-i,-s) must be specified");
			usage();
		}
		fdi = ustdin();
	} else {
		fdi = uropen (str_arg(opt_i, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}

	no_tty(fdi);

   /* access solar image file */

	if (!got_opt(opt_s)) {
		fds = ustdin();
	} else {
		fds = uropen (str_arg(opt_s, 0));
		if (fds == ERROR) {
			error("can't open \"%s\"", str_arg(opt_s, 0));
		}
	}

	no_tty(fds);

   /* access mask image file */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

	no_tty(fds);

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* process args */

	zenith = got_opt (opt_z);

   /* read headers of input images/write output headers */

	headers (fdi, fds, fdm, fdo, zenith);

   /* do it */

	gshade (fdi, fds, fdm, fdo, zenith);

   /* all done */

	ipwexit(EXIT_SUCCESS);
}
