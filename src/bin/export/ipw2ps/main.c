#include "ipw.h"

#include "pgm.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_r = {
		'r', "rotate output image 90 degrees",
	};

	static OPTION_T opt_h = {
		'h', "output image height (inches)",
		REAL_OPTARGS, "height",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_w = {
		'w', "output image width (inches)",
		REAL_OPTARGS, "width",
		OPTIONAL, 1, 1
	};

	static OPTION_T operands = {
		OPERAND, "input image file",
		STR_OPERANDS, "image",
		OPTIONAL, 0, 1,
	};

	static OPTION_T *optv[] = {
		&opt_r,
		&opt_h,
		&opt_w,
		&operands,
		0
	};

	int             fdi;		/* input image file descriptor	 */
	double          h_img;		/* PostScript image height	 */
	double          h_page;		/* PostScript page height	 */
	double          w_img;		/* PostScript image width	 */
	double          w_page;		/* PostScript page width	 */

 /*
  * begin
  */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);
 /*
  * collect options
  */
	h_page = PAGE_HEIGHT;
	w_page = PAGE_WIDTH;

	if (got_opt(opt_h)) {
		h_img = real_arg(opt_h, 0);

		if (h_img <= 0.0) {
			error("image height %g too small", h_img);
		}

		if (h_img > IMG_HEIGHT) {
			error("image height %g too large (max %g inches)",
			      h_img, IMG_HEIGHT);
		}
	}
	else {
		h_img = IMG_HEIGHT;
	}

	if (got_opt(opt_w)) {
		w_img = real_arg(opt_w, 0);

		if (w_img <= 0.0) {
			error("image width %g too small", h_img);
		}

		if (w_img > IMG_WIDTH) {
			error("image width %g too large (max %g inches)",
			      w_img, IMG_WIDTH);
		}
	}
	else {
		w_img = IMG_WIDTH;
	}
 /*
  * access input file
  */
	if (!got_opt(operands)) {
		fdi = ustdin();
	}
	else {
		fdi = uropen(str_arg(operands, 0));
		if (fdi == ERROR) {
			error("can't open \"%s\"", str_arg(operands, 0));
		}
	}

	no_tty(fdi);
 /*
  * do it
  */
	pspic(fdi, h_page, w_page, h_img, w_img, got_opt(opt_r));
 /*
  * all done
  */
	ipwexit(EXIT_SUCCESS);
}
