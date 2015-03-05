#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

 /*
 * band #'s for quantities in input file
 */

#define GLOBAL_BAND	0
#define DIFFUSE_BAND	1
#define CLOUD_BAND	2

 /*
 * # input and output bands
 */

#define NBANDS		3
#define OBANDS		2

 /*
 * coefs for beam ratio reduction
 */

#define CRAT1		0.15
#define CRAT2		0.99
#define CCOEF		1.38

extern int	head_init(int fdi, int fdo, int obands);
extern int	head_final(int fdo, fpixel_t *min_max, int obands);
extern int	cloudcalc(fpixel_t *in_buf, int n, fpixel_t *out_buf,
			int obands);
extern void	cf_cloud(int fdi, int fdo);

#endif
