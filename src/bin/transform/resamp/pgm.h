#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define MEAN_VAL	0
#define MIN_VAL		1
#define MAX_VAL		2

#define NO_DATA	-999999.0

extern void	headers(int fdi, int fdm, int fdo, int zline, int zsamp);
extern void	out_line(int fdo, fpixel_t *obuf, double *sums, int *npix,
			 int o_nsamps, int nbands, int ztype);
extern void	resamp(int fdi, int fdm, int fdo, int zline, int zsamp,
		       int ztype);
extern void	resamp_line(fpixel_t *ibuf, int fdm, pixel_t *mbuf, int i_nsamps,
			    fpixel_t *obuf, double *sums, int *npix,
			    int o_nsamps, int nbands, int zsamp, int ztype);

#endif
