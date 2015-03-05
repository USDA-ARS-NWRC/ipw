#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

extern void	accum(int fdi, int fdm, pixel_t *ibuf, pixel_t *mbuf,
		      int nlines, int nsamps, int *hist, int *pixcount);
extern void	read_headers(int fdi);

#ifdef GETARGS_H
extern void	check_headers(int nfiles, OPTION_T operands, int fdm,
			      int *nbits, int *nlines, int *nsamps);
extern void	cnhist(int nfiles, OPTION_T operangs, int fdm);
#endif

#endif
