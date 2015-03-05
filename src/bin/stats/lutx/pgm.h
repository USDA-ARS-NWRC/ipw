#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

extern void     lutx(int i_fd, int L_fd, int o_fd);
extern void     lutximg(int i_fd, int nlines, int nsamps,
			REG_6 int nbands, REG_3 pixel_t **lut, int o_fd);

#endif
