#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

extern void	mkgeoh(int i_fd, double *origin, double *incr, char *units,
		       char *csys, int h_nbands, int *h_band,
		       bool_t force, int o_fd);

#endif
