#include "ipw.h"

#include "pgm.h"

int
main(
	int argc,
	char **argv)
{
  static OPTION_T opt_verb = {
    'v', "verbose"
  };
  static OPTION_T opt_mask = {
    'm', "mask image",
    STR_OPTARGS, "mask",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_image = {
    OPERAND, "One IPW image",
    STR_OPERANDS, "image ",
    OPTIONAL, 1, 1
  };

  static OPTION_T *optv[] = {
    &opt_verb,
    &opt_mask,
    &opt_image,
    0
  };


  ipwenter(argc, argv, optv, IPW_DESCRIPTION);

  parm.verbose = (got_opt(opt_verb)) ? 1 : 0;

  if (got_opt(opt_mask)) {
    parm.imask = uropen (str_arg(opt_mask, 0));
    if (parm.imask == ERROR)
       error ("can't open \"%s\"", str_arg(opt_mask, 0));
  } else
    parm.imask = ERROR;

  if (got_opt(opt_image)) {
    if ( (parm.im = uropen (str_arg(opt_image, 0))) == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_image, 0));
    }
  } else
    parm.im = ustdin();

        /* done with option parsing */

  parm.output = ustdout();

  headers();

  if ( (parm.nbits <= 8) && (parm.imask == ERROR) )
    qipw2pgm();
  else
    ipw2pgm();

  ipwexit(EXIT_SUCCESS);
}
