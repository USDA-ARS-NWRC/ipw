#include "ipw.h"

#include "fpio.h"
#include "orh.h"
#include "pgm.h"

int
main(
	int             argc,
	char           **argv)
{
	static OPTION_T opt_s = {
		's', "compute slope image only (default both)"
	};

	static OPTION_T opt_a = {
		'a', "compute aspect image only"
	};

	static OPTION_T opt_d = {
		'd',
		"grid spacing (line & samp, normally gotten from GEOH header)",
		REAL_OPTARGS, "delta",
		OPTIONAL, 1, 2
	};

	static OPTION_T opt_i = {
		'i',
		"# bits in output linear quantization (default CHAR_BIT)",
		INT_OPTARGS, "bits",
		OPTIONAL, 1, 2
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
		&opt_d,
		&operand,
		0
	};

	int             fdi = 0;	/* input image file descriptor	 */
	int             fdo = 0;	/* output image file descriptor	 */
	int             nbits[2];	/* # bits in output LQ's	 */
	bool_t          s;		/* ? compute slopes		 */
	bool_t          a;		/* ? compute aspects		 */
	fpixel_t        spacing[2];	/* grid spacing			 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * access file
  */
	switch (n_args(operand)) {

	case 0:			/* no operand, open std input	 */
		fdi = ustdin();
		break;

	case 1:			/* open named file		 */
		fdi = uropen(str_opnd(operand, 0));
		if (fdi == ERROR)
			error("Can't open \"%s\"", str_opnd(operand, 0));
		break;

	default:			/* too many operands		 */
		usage();
	}
 /*
  * can't read or write tty
  */
	no_tty(fdi);
	fdo = ustdout();
	no_tty(fdo);
 /*
  * parse options
  */
	options(&opt_s, &opt_a, &opt_d, &opt_i, &s, &a, nbits, spacing);
 /*
  * read/write headers, either use spacing provided or get from
  * geodetic header
  */
	headers(fdi, fdo, s, a, nbits, spacing);
 /*
  * read elevation data, compute partial x & partial y, compute and
  * write gradient
  */
	gradient(fdi, fdo, s, a, spacing);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
