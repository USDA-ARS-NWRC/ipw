#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "lqh.h"

extern LQH_T	**newlqh(int fdi, int fdo);
extern void	headers(int fdi, int fds, int fdm, int fdo, bool_t zenith);
extern void	gshade(int fdi, int fds, int fdm, int fdo, bool_t zenith);
extern void	trigtbl(int fdi, int fds, fpixel_t *sintbl, float *costbl,
			float *zsintbl, float *zcostbl, bool_t zenith);

#endif
