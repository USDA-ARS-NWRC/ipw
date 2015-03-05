#include "ipw.h"
#include "pgm.h"
#include "compile.h"
#include "token.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_expr = {
		"expr", "arithmetic expression",
		STR_OPTARGS, "string",
		OPTIONAL, ONE_ARG_WITH_COMMAS,
	};

	static OPTION_T opt_file = {
		"file", "text file with arithmetic expression",
		STR_OPTARGS, "string",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_minmax = {
		"minmax", "minimum and maximum for LQ header",
		REAL_OPTARGS, "min,max",
		REQUIRED, 2, 2,
	};

	static OPTION_T opt_units = {
		"units", "units for LQ header",
		STR_OPTARGS, "string",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_bits = {
		"bits", "# of bits in output image (default: 8)",
		INT_OPTARGS, "n",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_mask = {
		"mask", "mask image",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_lines = {
		"lines", "# of lines in output image (default: 1)",
		INT_OPTARGS, "n",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_samps = {
		"samps", "# of samples in output image (default: 1)",
		INT_OPTARGS, "n",
		OPTIONAL, 1, 1,
	};

	static OPTION_T opt_names = {
		"names", "names for bands",
		STR_OPTARGS, "name",
		OPTIONAL, 1,
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_expr,
		&opt_file,
		&opt_minmax,
		&opt_units,
		&opt_bits,
		&opt_mask,
		&opt_lines,
		&opt_samps,
		&opt_names,
		&operands,
		NULL
	};

	char	*i_image_name;	/* filename of input image		*/
	int	nbits;		/* # of bits for output image's band	*/
	int	i;


	/*
	 *  Begin
	 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);


	/*
	 *  Make sure list of band names is valid.
	 */
	if (got_opt(opt_names)) {
		bandNames = str_argp(opt_names);
		nameCount = n_args(opt_names);
		for (i = 0; i < nameCount; i++) {
			if (! validIdentifier(bandNames[i])) {
			    error("Invalid band name: \"%s\"", bandNames[i]);
			}
		}
	}

	/*
	 *  Compile the expression into interpreter commands.
	 *  Need just one of the -expr and -file options.
	 */
	opt_check(1, 1, 2, &opt_expr, &opt_file);
	if (got_opt(opt_expr)) {
	    num_cmds = compile(str_arg(opt_expr,0), TRUE, cmd_buf,CMD_BUF_LEN);
	} else {
	    num_cmds = compile(str_arg(opt_file,0), FALSE, cmd_buf,CMD_BUF_LEN);
	}
	if (num_cmds == 0) {
		error("empty expression");
	} else if (num_cmds < 0) {
		if (tokenType == TOKEN_EOI) {
			error("at end of the expression:");
		} else if (isprint(*tokenText)) {
			error("at the %d%s character ('%c') in the expression:",
			      (tokenPosition + 1), ordinal(tokenPosition+1),
			      *tokenText);
                } else {
                	error("at the %d%s character in the expression:",
			      (tokenPosition + 1), ordinal(tokenPosition+1));
		}
	}

	/*
	 *  Access input image
	 */
	if (got_opt(operands)) {
		i_image_name = str_arg(operands, 0);
		i_image = open_image(i_image_name, FPIXEL_ACCESS);
		if (i_image == NULL) {
			error("can't open input image \"%s\"", i_image_name);
		}
	} else if (constantExpr) {
		/*
		 *  We have a constant expression, but no input image.
		 *  Therefore, check if the # of lines and samples specified
		 *  on command line.
		 */
		if (got_opt(opt_lines)) {
			num_lines = int_arg(opt_lines, 0);
			if (num_lines < 1)
				error("number of lines less than 1");
		} else {
			num_lines = 1;
		}
		if (got_opt(opt_samps)) {
			num_samples = int_arg(opt_samps, 0);
			if (num_samples < 1)
				error("number of samples less than 1");
		} else {
			num_samples = 1;
		}
		i_image = NULL;
	} else {
		i_image = open_image(IPW_STDIN, FPIXEL_ACCESS);
		if (i_image == NULL) {
			error("can't access standard input for input image");
		}
	}

	/*
	 *  If we have an input image, get its dimensions.
	 */
	if (i_image != NULL) {
		not_a_tty(i_image);

		num_lines = nlines(i_image);
		num_samples = nsamples(i_image);
		num_bands = nbands(i_image);

		if (got_opt(opt_lines))
			error("option \"-lines\" specified with input image");
		if (got_opt(opt_samps))
			error("option \"-samps\" specified with input image");
	}

	/*
	 *  Access mask image
	 */
	if (got_opt(opt_mask)) {
		mask = open_mask(str_arg(opt_mask, 0), num_lines, num_samples);
	} else {
		mask = (IMAGE_T *) NULL;
	}

	/*
	 *  Create output file
	 */
	if (got_opt(opt_bits)) {
		nbits = int_arg(opt_bits, 0);
		if ((nbits < 1) || (nbits > PIXEL_BIT)) {
			error("nbits must be between 1 and %d", PIXEL_BIT);
		}
	} else {
		nbits = 8;
	}

	o_image = new_image(IPW_STDOUT, num_lines, num_samples, 1, &nbits);
	if (o_image == NULL) {
		error("can't access standard output for output image");
	}
	not_a_tty(o_image);

	/*
	 *  Process headers
	 */
	headers(real_arg(opt_minmax, 0), real_arg(opt_minmax, 1),
		got_opt(opt_units) ? str_arg(opt_units, 0) : NULL);

	/*
	 *  Process the input image.
	 */
	imath();

	if (i_image != NULL)
		close_image(i_image);
	if (mask != NULL)
		close_image(mask);
	close_image(o_image);

	ipwexit(EXIT_SUCCESS);
}
