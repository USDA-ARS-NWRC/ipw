#ifndef	PGM_H
#define	PGM_H

#include "../transpose.new/IPWmacros.h"

extern void	transpose(int i_fd, int o_fd);
extern void	ximg(fpixel_t *i_buf, int i_nlines, int i_nsamps, fpixel_t *o_buf);

#endif

