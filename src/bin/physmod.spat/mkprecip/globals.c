/*
 *  Global variables for "mkprecip" program.
 */

#include "ipw.h"
#include "pgm.h"

IMAGE_T	*  i_image;		/* input image			 */
IMAGE_T *  o_image;		/* output image			 */

int	   num_lines;		/* # of lines in image		 */
int	   num_samples;		/* # of samples per line	 */
int	   num_bands;		/* # of bands per sample	 */

