#include "ipw.h"

#include "fpio.h"
#include "orh.h"
#include "pgm.h"

main(
	int             argc,
	char           *argv[])
{
	static OPTION_T opt_n = {
		'n',
		"# of bands to interpolate",
		INT_OPTARGS, "bands",
		REQUIRED, 1, 1
	};

	static OPTION_T operand = {
		OPERAND, "input 2-band image of interpolation endpoints",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_n,
		&operand,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	int             fdo;		/* output image file descriptor	 */
	int		outbands;	/* # bands to interpolate        */	

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
	outbands = int_arg(opt_n, 0);

 /*
  * read/write headers
  */
	headers(fdi, fdo, outbands);
 /*
  * interpolate   
  */
	fillbands(fdi, fdo, outbands);
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
