#ifndef	PGM_H
#define	PGM_H

#include "../skew.new/IPWmacros.h"

#define	SCR_PREFIX	"skew"		/* scratch file name prefix	*/

extern void run_skew(int i_fd, bool_t fwd, double angle,
                              bool_t skip_skewh, int nthreads,int o_fd);
extern void skew(fpixel_t *image, int nlines, int nsamps, int nbands,
		bool_t fwd, double angle, int nthreads, int *o_nsamps, fpixel_t **oimage);

#endif
