#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

extern void	transpose(int i_fd, int o_fd);
extern void	ximg(int i_fd, int i_nlines, int i_nsamps, REG_6 int samp_nbytes,
		     int o_fd);

#endif

