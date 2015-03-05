#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_s = {
		's', "compute slope image only (default both)"
	};

	static OPTION_T opt_a = {
		'a', "compute aspect image only"
	};

	static OPTION_T opt_i = {
		'i',
		"# bits in output linear quantization (default CHAR_BIT)",
		INT_OPTARGS, "bits",
		OPTIONAL, 1, 2
	};

	static OPTION_T opt_m = {
		'm', "mask image",
		STR_OPERANDS, "mask",
		OPTIONAL, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input elevation image",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_s,
		&opt_a,
		&opt_i,
		&opt_m,
		&operand,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdm;		/* mask image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int             nbits[2];	/* # bits in output LQ's	 */
	bool_t          slope;		/* ? compute slopes		 */
	bool_t          aspect;		/* ? compute aspects		 */
	double		blat;		/* Northernmost latitude (deg)	 */
	double		blon;		/* Westernmost longitude (deg)	 */
	double		dlat;		/* latitude spacing (deg)	 */
	double		dlon;		/* longitude spacing (deg)	 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* access file */

	fdi = ERROR;
	switch (n_args(operand)) {

	case 0:			/* no operand, open std input	 */
		fdi = ustdin();
		break;

	case 1:			/* open named file		 */
		fdi = uropen (str_opnd(operand, 0));
		if (fdi == ERROR)
			error ("Can't open \"%s\"", str_opnd (operand, 0));
		break;

	default:			/* too many operands		 */
		usage ();
	}

   /* access mask */

	if (got_opt(opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR)
			error ("Can't open \"%s\"", str_arg (opt_m, 0));
	} else {
		fdm = ERROR;
	}

   /* can't read or write tty */

	no_tty(fdi);
	fdo = ustdout();
	no_tty(fdo);

   /* process options */

	slope = got_opt(opt_s);
	aspect = got_opt(opt_a);

   /* default is to do both */

	if (!slope && !aspect) {
		slope = aspect = TRUE;
	}

    /* # bits in output image */

	if (got_opt(opt_i)) {
		nbits[0] = int_arg(opt_i, 0);
		if (nbits[0] <= 0) {
			error ("# bits must be >= 1");
		}
		if (n_args(opt_i) > 1) {
			nbits[1] = int_arg(opt_i, 1);
			if (nbits[1] <= 0) {
				error ("# bits must be >= 1");
			}
		} else {
			nbits[1] = nbits[0];
		}
	} else {
		nbits[0] = nbits[1] = CHAR_BIT;
	}

   /* read/write headers, get lat/lon corner and spacings from geodetic header */

	headers (fdi, fdm, fdo, slope, aspect, nbits, &blat, &blon, &dlat, &dlon);

   /* read elevation data, compute partial x & partial y, compute and */
   /* write gradient */

	ggradient (fdi, fdm, fdo, slope, aspect, blat, blon, dlat, dlon);

   /* all done */

	ipwexit (EXIT_SUCCESS);
}
