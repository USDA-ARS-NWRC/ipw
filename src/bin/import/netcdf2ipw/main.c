#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_l = {
		'l', "# lines in output image",
		INT_OPTARGS, "nlines",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
		's', "# samples in output image",
		INT_OPTARGS, "nsamps",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_b = {
		'b', "# bands in output image",
		INT_OPTARGS, "nbands",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "# bits per band in output image (default: 8)",
		INT_OPTARGS, "nbits",
		OPTIONAL, 1
	};

	static OPTION_T operand = {
		OPERAND, "ASCII input file",
		STR_OPERANDS, "infile",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_l,
		&opt_s,
		&opt_b,
		&opt_n,
		&operand,
		0
	};

	FILE	       *ifp;		/* input file pointer		 */
	int             fdo;		/* output image file descriptor	 */
	int		nlines;		/* # lines in output image	 */
	int		nsamps;		/* # samps in output image	 */
	int		nbands;		/* # bands in output image	 */
	int	       *nbits;		/* # bits/band in output image	 */
	int		i;		/* loop counter			 */


   /* begin */

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

   /* process args */
	
	/* avoid warning */
	ifp = 0;
	nlines = int_arg (opt_l, 0);
	nsamps = int_arg (opt_s, 0);
	if (got_opt (opt_b)) {
		nbands = int_arg (opt_b, 0);
	} else {
		nbands = 1;
	}

   /* access input file -- ifp is a FILE *, not an IPW uio file */

	switch (n_args(operand)) {
	case 0:
		ifp = stdin;
		break;
	case 1:
		ifp = fopen(str_opnd(operand, 0), "r");
		if (ifp == NULL)
			error("Can't open \"%s\"", str_opnd(operand, 0));
		break;
	case 2:
		usage();
	}

   /* access output file */

	fdo = ustdout();
	no_tty(fdo);

   /* allocate/initialize nbits array */

	nbits = (int *) ecalloc (nbands, sizeof(int));
	if (nbits == NULL) {
		error ("can't allocate nbits array");
	}

	switch (n_args(opt_n)) {
	case 0:
		for (i = 0; i < nbands; i++)
			nbits[i] = DFLT_NBITS;
		break;
	case 1:
		for (i = 0; i < nbands; i++)
			nbits[i] = int_arg (opt_n, 0);
		break;
	default:
		if (n_args(opt_n) == nbands) {
		  for (i = 0; i < nbands; i++)
		    nbits[i] = int_arg (opt_n, i);
		} else {
		 error("must specify #bits for each band or one value for all");
		}
		break;
	}

   /* write headers of output file */

	headers (fdo, nlines, nsamps, nbands, nbits);

   /*
    * iimport and output -- quick_import reads one band inputs, and does not
    * write to a temporary file.  Since it keeps all the data in core, the
    * input file must not be too large.  Images less than 1024x1024 should
    * keep the memory requirements below 4 megabytes, which is acceptable.
    */

	if ( (nbands == 1) && ( (nlines*nsamps) < 1000000 ) ) {
	  quick_import (ifp, fdo, nlines, nsamps, nbands);
	} else {
	  iimport (ifp, fdo, nlines, nsamps, nbands);
	}

   /* all done */

	ipwexit(EXIT_SUCCESS);
}
