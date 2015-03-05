#include "ipw.h"
#include "pgm.h"


int
main(
	int	argc,
	char  **argv)
{
  double  * ax;
  double  * ay;

  static OPTION_T opt_verb = {
    'v', "verbose"
  };
  static OPTION_T opt_show = {
    'g', "show intermediate approximations"
  };
  static OPTION_T opt_ts = {
    's', "use less memory at expense of speed"
  };
  static OPTION_T opt_order = {
    'o', "least sqaures polynomial order",
    LONG_OPTARGS, "order",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_lsmax = {
    'M', "maximum least sqaures polynomial order",
     LONG_OPTARGS, "order",
     OPTIONAL, 1, 1
  };
  static OPTION_T opt_tol = {
    't', "select order when error % changes less than this amount",
    REAL_OPTARGS, "tolerance",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_clip = {
    'c', "clipping values",
    REAL_OPTARGS, "min,max",
    OPTIONAL, 2, 2
  };
  static OPTION_T opt_mask = {
    'm', "mask image",
    STR_OPTARGS, "mask",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_images = {
    OPERAND, "images",
    STR_OPERANDS, "images",
    OPTIONAL, 1
  };

  static OPTION_T *optv[] = {
    &opt_verb,
    &opt_show,
    &opt_ts,
    &opt_order,
    &opt_lsmax,
    &opt_tol,
    &opt_clip,
    &opt_mask,
    &opt_images,
    0
  };


  ipwenter(argc, argv, optv, IPW_DESCRIPTION);

  if (got_opt(opt_verb))
    parm.verbose = 1;
  else
    parm.verbose = 0;

  if (got_opt(opt_show))
    parm.show = 1;
  else
    parm.show = 0;

  if (got_opt(opt_ts))
    parm.space = 1;
  else
    parm.space = 0;

  if (got_opt(opt_mask)) {
    parm.imask = uropen (str_arg(opt_mask, 0));
    if (parm.imask == ERROR) {
       error ("can't open \"%s\"", str_arg(opt_mask, 0));
    }
  } else
    parm.imask = ERROR;

  if (got_opt(opt_images)) {
    parm.nfiles = n_args(opt_images);
    if (parm.nfiles != 2) {
      error ("istatrel currently supports only 2 images");
    }

    parm.im1 = uropen (str_arg(opt_images, 0));
    parm.im2 = uropen (str_arg(opt_images, 1));

    if (parm.im1 == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_images, 0));
    }
    if (parm.im2 == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_images, 1));
    }
  } else
    error("istatrel needs input images.  use istatrel -H for help");

  if (got_opt(opt_clip)) {
    parm.doclip = 1;
    parm.clipmin = real_arg(opt_clip, 0);
    parm.clipmax = real_arg(opt_clip, 1);
  } else
    parm.doclip = 0;

  if (got_opt(opt_tol)) {
    parm.tolerance = real_arg(opt_tol, 0);
  } else
    parm.tolerance = 0.01;

  if (got_opt(opt_order)) {
    parm.startd = long_arg(opt_order, 0);
    parm.maxd = parm.startd + 1;
  } else {
    parm.startd = 0;
    parm.maxd   = 20;
  }

  if (got_opt(opt_lsmax))
    parm.maxd = long_arg(opt_lsmax, 0) + 1;

        /* done with option parsing */

  headers();

  ax   = (double *) ecalloc(parm.maxn, sizeof(double));
  ay   = (double *) ecalloc(parm.maxn, sizeof(double));
  parm.cpoly = (double *) ecalloc(parm.maxd, sizeof(double));
  parm.ppoly = (double *) ecalloc(parm.maxd, sizeof(double));

  ipwload( ax, ay );

  stat_init( ax, ay );
 
  istatrel( ax, ay );

  SAFE_FREE((void *) ax);
  SAFE_FREE((void *) ay);
  SAFE_FREE((void *) parm.cpoly);
  SAFE_FREE((void *) parm.ppoly);

  ipwexit(EXIT_SUCCESS);
}
