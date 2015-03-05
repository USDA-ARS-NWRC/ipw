#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

void		mksath(int i_fd, char *platform, char *sensor, char *location,
		       char *gmdate, char *gmtime, int h_nbands, int *h_band,
		       bool_t force, int o_fd);

#endif
