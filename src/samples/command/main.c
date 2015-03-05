/*
**  Sample "main.c"
*/


/*
**  Include system header files.
*/

#include <...>
...


/*
**  IPW header files.
*/

#include "ipw.h"
#include "bih.h"
...
#include "pgm.h"


void
main(
	int      argc,
	char  ** argv)
{

/*
**  Define the program's options and operands.
**  See the man page for the function "ipwenter()" for details on
**  on defining options.
*/

/*
/*
**  "-a" option; no arguments
*/

	static OPTION_T	opt_a = {
		'a', "(option description)",
	};

/*
**  "-b" option; >= 3 integer arguments
*/

	static OPTION_T	opt_b = {
		'b', "(option description)",
		INT_OPTARGS, "(arg description)",
		OPTIONAL, 3,
	};

/*
**  "-c" option; >= 1 real argument; must be specified
*/

	static OPTION_T	opt_c = {
		'c', "(option description)",
		REAL_OPTARGS, "(arg description)",
		REQUIRED,
	};

/*
**  "-d" option; >= 1 and <= 4 string arguments; must be specified
*/
	static OPTION_T	opt_d = {
		'd', "(option description)",
		STR_OPTARGS, "(arg description)",
		REQUIRED, 1, 4
	};


/*
**  Define any operands (currently only string operands supported)
*/
	static OPTION_T operands = {
		OPERAND, "input image",
		STR_OPERANDS, "image",
		OPTIONAL, 1, 1
	};


/*
**  Define the array "optv" to hold pointers to the options and operands
*/
	static OPTION_T	*optv[] = {
		&opt_a,
		&opt_b,
		&opt_c,
		&opt_d,
		&operands,
		NULL
	};


/*
**  Define any other local variables.
*/

	int     i;	/* loop index 		    */
	float	sum;	/* sum of designated pixels */


/*
**  First, statement should be call to "ipwenter()".
*/
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

/*
**  Open input image (default is standard input), and output image.
**  Make sure neither one is a terminal.
*/
	if (got_opt(operands)) {
		I_fd = uropen(str_arg(operands, 0));
		if (I_fd == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}
	else {
		I_fd = ustdin();
	}
	no_tty(I_fd);

	O_fd = ustdout();
	no_tty(O_fd);

/*
**  Process image headers, and then the pixel data.
*/
	headers();
	@PGM@();

/*
**  Last statement should be a call to "ipwexit()".
*/
	ipwexit(EXIT_SUCCESS);
}
