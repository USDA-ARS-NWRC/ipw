#include "ipw.h"
#include "pgm.h"

static int o_bits[OBANDS] = { 8 }; /* # of bits per band */

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

	static OPTION_T opt_t = {
		't', "file with interpolation table",
		STR_OPERANDS, "file",
		REQUIRED, 1, 1,
	};

	static OPTION_T opt_n = {
		'n', "# of bits in output image's band",
		INT_OPERANDS, "nbits",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_t,
		&opt_n,
		&operands,
		0
	};

	char	*i_image_name;	/* filename of input image */


	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*
	 *  Load the interpolation table.
	 */
	load_table(str_arg(opt_t,0));

	/*
	 *  Open input file
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
	 *  Make sure the number of bands in input image matches the
	 *  number of dimensions in the interpolation table.
	 */
	if (num_bands != NumDimensions) {
	    error("# of input bands (%d) not = # of dimensions in table (%d)",
		  num_bands, NumDimensions);
	}

	/*
	 *  Create output file
	 */
	if (got_opt(opt_n)) {
	    o_bits[0] = int_arg(opt_n, 0);
	    if (o_bits[0] < 2) {
		error("# of bits for output image (%d) < 2", o_bits[0]);
	    }
	}

	o_image = new_image(IPW_STDOUT, num_lines, num_samples, OBANDS, o_bits);
	if (o_image == NULL) {
	    error("can't access standard output for output image");
	}
	not_a_tty(o_image);


	headers();
	interp_image();

	close_image(i_image);
	close_image(o_image);

	ipwexit(EXIT_SUCCESS);
}
