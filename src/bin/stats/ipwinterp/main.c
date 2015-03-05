#include "ipw.h"
#include "pgm.h"

int
main(
	int    argc,
	char **argv)
{
  double  * poly;

  static OPTION_T opt_verb = {
    'v', "verbose"
  };
  static OPTION_T opt_anal = {
    'a', "analyze statistics only"
  };
  static OPTION_T opt_bits = {
    'b', "bits in output image (default: 8)",
     INT_OPTARGS, "bits",
     OPTIONAL, 1, 1
  };
  static OPTION_T opt_mask = {
    'm', "mask image",
    STR_OPTARGS, "mask",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_image = {
    OPERAND, "image",
    STR_OPERANDS, "image",
    OPTIONAL, 1
  };

  static OPTION_T *optv[] = {
    &opt_verb,
    &opt_anal,
    &opt_bits,
    &opt_mask,
    &opt_image,
    0
  };


  ipwenter(argc, argv, optv, IPW_DESCRIPTION);

  scanf("%ld\n", &parm.needfiles);
  scanf("%ld\n", &parm.degree);

  if (got_opt(opt_verb))
    parm.verbose = 1;
  else
    parm.verbose = 0;

  if (got_opt(opt_anal))
    parm.analyze = 1;
  else
    parm.analyze = 0;

  if (got_opt(opt_bits))
    parm.nbits = int_arg(opt_bits, 0);
  else
    parm.nbits = 8;

  if (got_opt(opt_mask)) {
    parm.maskfile = uropen (str_arg(opt_mask, 0));
    if (parm.maskfile == ERROR) {
       error ("can't open \"%s\"", str_arg(opt_mask, 0));
    }
  } else
    parm.maskfile = ERROR;

  if (got_opt(opt_image)) {
    parm.nfiles = n_args(opt_image);
    if (parm.nfiles != parm.needfiles) {
      error ("this relation requires %ld images", parm.needfiles);
    }

    parm.im1 = uropen (str_arg(opt_image, 0));

    if (parm.im1 == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_image, 0));
    }
  } else
    if (!parm.analyze)
      error("Must have at least 1 image");

        /* done with option parsing */

  poly = (double *) ecalloc(parm.degree + 2, sizeof(double));

  read_par( poly );
 
  if (parm.analyze || parm.verbose)
    stat_anal();
  if (parm.analyze)
    ipwexit(EXIT_SUCCESS);

  parm.output = ustdout();
  no_tty(parm.output);

  headers();

  interp( poly );

  ipwexit(EXIT_SUCCESS);
}
