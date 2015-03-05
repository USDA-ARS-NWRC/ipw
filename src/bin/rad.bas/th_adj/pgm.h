#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

 /*
 * band #'s for quantities in input file
 */

#define THERMALIN_BAND	0
#define AIRTEMP_BAND	1
#define FACTOR_BAND	2

 /*
 * # input and output bands
 */

#define NBANDS		3
#define OBANDS		1

extern int	head_init(int fdi, int fdo, int obands);
extern int	head_final(int fdo, fpixel_t *min_max, int obands);
extern int	adjust(fpixel_t *in_buf, int n, fpixel_t *out_buf,
			int obands);
extern void	th_adj(int fdi, int fdo);

#endif
