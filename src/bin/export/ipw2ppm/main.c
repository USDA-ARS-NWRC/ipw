#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

int
main(
	int	argc,
	char  **argv)
{
  static OPTION_T opt_verb = {
    'v', "verbose"
  };
  static OPTION_T opt_cbar = {
    'b', "color bar"
  };
  static OPTION_T opt_gray = {
    'g', "graymap"
  };
  static OPTION_T opt_pgmo = {
    'G', "force PGM"
  };
  static OPTION_T opt_color = {
    'c', "colormap file",
    STR_OPTARGS, "colormap-file",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_mask = {
    'm', "mask image",
    STR_OPTARGS, "mask",
    OPTIONAL, 1, 1
  };
  static OPTION_T opt_images = {
    OPERAND, "1 or 3 band IPW image",
    STR_OPERANDS, "image ",
    OPTIONAL, 1, 1
  };

  static OPTION_T *optv[] = {
    &opt_verb,
    &opt_cbar,
    &opt_gray,
    &opt_pgmo,
    &opt_color,
    &opt_mask,
    &opt_images,
    0
  };


  ipwenter(argc, argv, optv, IPW_DESCRIPTION);

  parm.verbose = (got_opt(opt_verb)) ? 1 : 0;

  parm.cbar =    (got_opt(opt_cbar)) ? 1 : 0;

  parm.graymap = (got_opt(opt_gray)) ? 1 : 0;

  parm.forcepgm =(got_opt(opt_pgmo)) ? 1 : 0;

  if (got_opt(opt_mask)) {
    parm.imask = uropen (str_arg(opt_mask, 0));
    if (parm.imask == ERROR)
       error ("can't open \"%s\"", str_arg(opt_mask, 0));
  } else
    parm.imask = ERROR;

  if (got_opt(opt_color)) {
    if (parm.graymap) {
	warn ("Colormap requested with a graymap.  Turning off graymap.");
	parm.graymap = 0;
    }
    parm.cmfile = fopen (str_arg(opt_color, 0), "r");
    if (parm.cmfile == NULL)
       error ("can't open \"%s\"", str_arg(opt_color, 0));
  } else
    parm.cmfile = NULL;

  if (got_opt(opt_images)) {
    parm.nfiles = n_args(opt_images);
    if ( (parm.nfiles != 1) && (parm.nfiles != 3) ) {
      error ("ipw2ppm takes only one image.");
    }
    if (parm.nfiles != 1) {
      error ("ipw2ppm currently supports only one image.  mux your images together.");
    }

    if ( (parm.im1 = uropen (str_arg(opt_images, 0))) == ERROR) {
      error ("can't open \"%s\"", str_arg(opt_images, 0));
    }
  } else {
    parm.nfiles = 1;
    parm.im1 = ustdin();
  }

        /* done with option parsing */

  parm.output = ustdout();

  initcolors();

  headers();

  if (parm.nbands == 3) {
    if (parm.cbar)
      error ("Colorbars are not supported on 3 band images.");
    if (parm.graymap)
      error ("Graymap scaling is not supported on 3 band images.");
    if (parm.cmfile != NULL)
      error ("3 band images are RGB mapped, not color mapped.");
    if (parm.forcepgm)
      error ("3 band images are always written in PPM format.");
    ipw3ppm();
  } else {
    ipwload();
    readcolormap();

    if (parm.graymap || parm.forcepgm)
      pgmwrite();
    else
      ppmwrite();

    (void) free (parm.imd);
    if (parm.imask != ERROR)
      (void) free (parm.maskd);
    (void) free (parm.pbins);
    (void) free (parm.CLUT);
  }

  if (parm.fpm == NULL) {
    (void) pxclose(parm.im1);
  } else {
    (void) fpclose(parm.im1);
  }
  if (parm.imask != ERROR)
    (void) pxclose(parm.imask);

  ipwexit(EXIT_SUCCESS);
}
