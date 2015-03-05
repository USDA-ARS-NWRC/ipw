#ifndef	PGM_H
#define	PGM_H

/*
 * header file for "mkprecip" program
 */

#include "IPWmacros.h"

#include "image.h"

#define IBANDS	2
#define OBANDS	4

extern	IMAGE_T *  i_image;		/* input image			 */
extern	IMAGE_T *  o_image;		/* output image			 */

extern	int	   num_lines;		/* # of lines in image		 */
extern	int	   num_samples;		/* # of samples per line	 */
extern	int	   num_bands;		/* # of bands per sample	 */


extern	void	calc_snow (float T_a, float * percent_snow, float * rho_snow);
extern	void	headers   (void);
extern	void	mkprecip  (void);

#endif
