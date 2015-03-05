#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#include "ipw.h"
#include "fpio.h"

extern void     head1(int fdi, int nbits, int ncoef, int fdo);
extern void     head2(int fdo, fpixel_t *fval);
extern void     wedge(int fdi, int nbits, aint_t ncoef, areal_t *cval,
		      int fdo);

#endif
