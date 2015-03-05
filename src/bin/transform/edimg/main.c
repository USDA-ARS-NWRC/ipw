#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_r = {
		'r', "replacement values are raw (i.e., not floating-point)",
	};

	static OPTION_T opt_k = {
		'k', "default replacement pixel value",
		REAL_OPTARGS, "const",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "coordinate file",
		STR_OPTARGS, "coords",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "input image",
		STR_OPTARGS, "image",
		OPTIONAL, 1, 1
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&opt_k,
		&opt_c,
		&opt_i,
		0
	};

	double          k;		/* default replacement value	 */
	int             c_fd;		/* coordinate file descriptor	 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	opt_check(1, 2, 2, &opt_c, &opt_i);
 /*
  * process options
  */
	k = got_opt(opt_k) ? real_arg(opt_k, 0) : DFLT_CONST;
 /*
  * access input image file
  */
	if (!got_opt(opt_i)) {
		i_fd = ustdin();
	}
	else {
		i_fd = uropen(str_arg(opt_i, 0));
		if (i_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_i, 0));
		}
	}

	no_tty(i_fd);
 /*
  * access coordinate file
  */
	if (!got_opt(opt_c)) {
		c_fd = ustdin();
	}
	else {
		c_fd = uropen(str_arg(opt_c, 0));
		if (c_fd == ERROR) {
			error("can't open \"%s\"", str_arg(opt_c, 0));
		}
	}
 /*
  * access output file
  */
	o_fd = ustdout();
	no_tty(o_fd);
 /*
  * do it
  */
	edimg(i_fd, c_fd, got_opt(opt_r), k, o_fd);
 /*
  * end
  */
	ipwexit(EXIT_SUCCESS);
}
