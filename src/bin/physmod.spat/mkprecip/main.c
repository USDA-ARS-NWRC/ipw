#include "ipw.h"
#include "pgm.h"

static int o_bits[OBANDS] = { 12, 8, 8, 12 }; /* # of bits per band */

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&operands,
		0
	};

	char	*i_image_name;	/* filename of input image */


 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		i_image = open_image(IPW_STDIN, FPIXEL_ACCESS);
		if (i_image == NULL) {
			error("can't access standard input for input image");
		}
	}
	else {
		i_image_name = str_arg(operands, 0);
		i_image = open_image(i_image_name, FPIXEL_ACCESS);
		if (i_image == NULL) {
			error("can't open input image \"%s\"", i_image_name);
		}
	}

	not_a_tty(i_image);

	num_lines = nlines(i_image);
	num_samples = nsamples(i_image);
	num_bands = nbands(i_image);

 /*
  *  Make sure the input image has the right # of input bands
  */
	if (num_bands != IBANDS) {
		error("input image must have %d bands", IBANDS);
	}

 /*
  * access output file
  */
	o_image = new_image(IPW_STDOUT, num_lines, num_samples, OBANDS, o_bits);
	if (o_image == NULL) {
		error("can't access standard output for output image");
	}
	not_a_tty(o_image);

 /*
  * do it
  */
	headers();
	mkprecip();

	close_image(i_image);
	close_image(o_image);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
