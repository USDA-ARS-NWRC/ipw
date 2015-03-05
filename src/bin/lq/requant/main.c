
#include "ipw.h"
#include "pgm.h"

int
main(
	int argc,
	char **argv)
{
  static OPTION_T opt_lqh = {
    'h', "new linear quantization header",
    STR_OPTARGS, "lqh",
    OPTIONAL, 1, 1,
  };
  static OPTION_T opt_lohi = {
    'm', "new low and high floating point values",
    REAL_OPTARGS, "val",
    OPTIONAL, 2, 2,
  };
  static OPTION_T opt_bits = {
    'b', "number of bits per pixel in output image",
    INT_OPTARGS, "bits",
    OPTIONAL, 1, 1,
  };
  static OPTION_T opt_verb = {
    'v', "display quantization headers during run"
  };

  static OPTION_T opt_image = {
    OPERAND, "input image file",
    STR_OPERANDS, "image",
    OPTIONAL, 1, 1
  };

  static OPTION_T *optv[] = {
    &opt_lqh,
    &opt_lohi,
    &opt_bits,
    &opt_verb,
    &opt_image,
    0
  };

  int      fdi;
  int      fdh;
  int      fdo;
  fpixel_t newmm[2];
  int      newbits;

  ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/* access input LQH */

  if (!got_opt(opt_lqh)) {
    fdh = ERROR;
  } else {
    fdh = uropen(str_arg(opt_lqh, 0));
    if (fdh == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_lqh, 0));
    }
  }

	/* access -m option */

  if (!got_opt(opt_lohi)) {
    newmm[0] = 0.0;
    newmm[1] = 0.0;
  } else {
    if (got_opt(opt_lqh)) {
      error("can't use both an LQH header and -m option");
    }
    newmm[0] = real_arg(opt_lohi, 0);
    newmm[1] = real_arg(opt_lohi, 1);
  }

	/* access -b option */

  if (!got_opt(opt_bits)) {
    newbits = 0;
  } else {
    if (got_opt(opt_lqh)) {
      error("can't use both an LQH header and -b option");
    }
    newbits = int_arg(opt_bits, 0);
  }

	/* access input image */

  if (!got_opt(opt_image)) {
    fdi = ustdin();
  } else {
    fdi = uropen(str_arg(opt_image, 0));
    if (fdi == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_image, 0));
    }
  }

  no_tty(fdi);

  fdo = ustdout();
  no_tty(fdo);

  requant(fdi, got_opt(opt_lqh), fdh,
               got_opt(opt_lohi), newmm,
               got_opt(opt_bits), newbits,
               got_opt(opt_verb),
          fdo);
  
  ipwexit(EXIT_SUCCESS);
}
