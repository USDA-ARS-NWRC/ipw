#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

/* band #'s for quantities in input file */

#define MU0_BAND	0
#define BEAM_BAND	1
#define DIFFUSE_BAND	2
#define MU_BAND		3
#define VF_BAND		4
#define CT_BAND		5
#define ALB_BAND	6
#define R0_BAND		7
#define NBANDS		8

extern void	gtoporad(int fdi, int fdo, bool_t net, int nbits);
extern int	head_init(int fdi, int fdo, int nbits);
extern int	head_final(int fdo, fpixel_t *rval);
extern int	radcalc(fpixel_t *in_buf, int n, bool_t net, fpixel_t *out_buf);

#endif
