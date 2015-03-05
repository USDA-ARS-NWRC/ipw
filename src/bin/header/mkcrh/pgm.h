#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define MAX_CLASS	255	/* max # classes per band */

extern void	mkcrh(int i_fd, int nclass, CLASS class[], pixel_t floor,
		      pixel_t ceil, char *annot, char *units, int h_nbands,
		      int *h_band, bool_t force, int o_fd);
extern void	get_classes(FILE *fp, int *nclass, CLASS *classes);

#endif
