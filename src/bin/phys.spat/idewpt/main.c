#include <math.h>
#include <omp.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "envphys.h"
#include "pgm.h"

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

	static OPTION_T opt_P = {
			'P', "Number of threads",
			REAL_OPTARGS, "threads",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_t = {
			't', "tolerance threshold",
			REAL_OPTARGS, "tolerance",
			OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
			&opt_m,
			&opt_P,
			&opt_t,
			&operand,
			0
	};

	int	fdi;		/* input image file descriptor	*/
	int	fdm;		/* mask image file descriptor	*/
	int	fdo;		/* output image file descriptor */
	int nthreads;	/* number of threads to use*/
	double tol;		/* tolerance threshold for zerobr */

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

	/* check number of threads to use */
	if (got_opt(opt_P)) {
		nthreads = real_arg(opt_P, 0);
		if (nthreads > omp_get_max_threads()){
			nthreads = omp_get_max_threads();
			printf("WARNING - maximum number of threads is %i, using %i\n", omp_get_max_threads(), nthreads);
		}
	}
	else {
		nthreads = 1;
	}

	/* check number of threads to use */
	if (got_opt(opt_t)) {
		tol = real_arg(opt_t, 0);
	}
	else {
		tol = 0.0;
	}

	/* can't read or write tty */

	no_tty (fdi);
	fdo = ustdout ();
	no_tty (fdo);

	/* read/write headers */

	headers (fdi, fdm, fdo, IBANDS, OBANDS);

	/* read input image, perform calculations and write output image */

	idewpt (fdi, fdm, fdo, nthreads, tol);

	/* all done */

	ipwexit (EXIT_SUCCESS);
}
