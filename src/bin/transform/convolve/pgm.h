#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

extern void         bufcycle(pixel_t **ibufs, int nbufs);
extern void         conv1d(pixel_t *ibuf, int nsamps, fpixel_t **kernel,
			   int ncols, fpixel_t *obuf);
extern void         convolve(int i_fd, FILE * kernelFile, int o_fd);
extern fpixel_t *** do_kmap(fpixel_t **kernel, int nrows, int ncols, 
			    fpixel_t *map, int maplen);
extern fpixel_t  ** getkernel(FILE * file, int *nrowsp, int *ncolsp);
extern void         putkernel(fpixel_t **kernel, int nrows, int ncols);

#endif
