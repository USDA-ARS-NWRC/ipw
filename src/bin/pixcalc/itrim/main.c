#include "ipw.h"
#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	float	min, max;		/* output min/max values */
	char	*strmin, *strmax;	/* string version of min,max */
	float   nodata;			/* requested nodata value */	
	float   nodatamin;		/* nodata val for vals < min */	
	float   nodatamax;		/* nodata val for vals > max */	
	

	static OPTION_T opt_v = {
		'v', "min,max values for output image (or 'x' for input min/max)",
		STR_OPTARGS, "val",
		REQUIRED, 2, 2
	};

	static OPTION_T opt_n = {
		'n', "nodata value (for values outside min,max) (default: 0.0)",
		REAL_OPTARGS, "nodata",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "cap mode: values < min get min; values > max get max.",
	};

	static OPTION_T opt_b = {
		'b', "binary mode: output image will be a 0-1 mask.",
	};

	static OPTION_T opt_p = {
		'p', "precision: # of output bits (Default: max of input bits)",
		INT_OPTARGS, "nbits",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1,
	};

	static OPTION_T *optv[] = {
		&opt_v,
		&opt_n,
		&opt_p,
		&opt_c,
		&opt_b,
		&operands,
		0
	};

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

 /*
  * access input file(s)
  */
	if (!got_opt(operands)) {
		parm.i_fd = ustdin();
	}
	else {
		parm.i_fd = uropen(str_arg(operands, 0));
		if (parm.i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(parm.i_fd);
 /*
  * set min,max
  */
	strmin = str_arg(opt_v, 0);
	strmax = str_arg(opt_v, 1);

	min = (strmin[0] == 'x') ? -FLT_MAX: atof(strmin);
	max = (strmax[0] == 'x') ? FLT_MAX : atof(strmax);

 /*
  * set nbits
  */
	if (got_opt(opt_p)) {
		parm.nbits = int_arg(opt_p, 0);
		if(parm.nbits <= 0) error("nbits must be >= 0.");
	} else {
		parm.nbits = 0;  /* flag value for head1() function */
	}
 /*
  * access output file
  */
	parm.o_fd = ustdout();
	no_tty(parm.o_fd);
 /*
  * process arguments
  */
	opt_check(0, 1, 3, &opt_c, &opt_b, &opt_n); /* only one of -c -b -n */

	parm.bin = got_opt(opt_b) ? 1 : 0;

	parm.cap = got_opt(opt_c) ? 1 : 0;
 /*
  * set nodata values
  */
	nodata = got_opt(opt_n) ? real_arg(opt_n, 0) : 0.0;

	nodatamin = got_opt(opt_c) ? min : nodata;
	nodatamax = got_opt(opt_c) ? max : nodata;

 /*
  * do it
  */
	headers(FALSE);
	scale(min, max, nodatamin, nodatamax);
	headers(TRUE);
	output();
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
