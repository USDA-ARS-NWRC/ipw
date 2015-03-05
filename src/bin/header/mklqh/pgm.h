#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

extern void	mklqh(int i_fd, int nbkpts, pixel_t *ibkpt, fpixel_t *fbkpt,
		      char *units, char *interp, int h_nbands,
		      int *h_band, bool_t force, int o_fd);

#endif
