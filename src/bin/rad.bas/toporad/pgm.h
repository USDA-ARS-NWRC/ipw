#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

 /*
 * band #'s for quantities in input file
 */

#define BEAM_BAND	0
#define DIFFUSE_BAND	1
#define MU_BAND		2
#define VF_BAND		3
#define CT_BAND		4
#define ALB_BAND	5
#define NBANDS		6

extern int	head_init(int fdi, int fdo, int obands, fpixel_t *mu0);
extern int	head_final(int fdo, fpixel_t *min_max, int obands);
extern int	radcalc(fpixel_t *in_buf, int n, bool_t net, fpixel_t mu0,
			fpixel_t *out_buf, int obands, bool_t got_R0, float R0);
extern void	toporad(int fdi, int fdo, bool_t net, bool_t got_R0,
			bool_t output_diff, float R0);

#endif
