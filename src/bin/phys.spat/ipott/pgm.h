#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define T_MIN		-45.0	/* minimum temp for LQ header	*/
#define T_MAX		 45.0	/* maximum temp for LQ header	*/

extern void	headers(int fdi, int fdm, int fd_elev, int inv, int fdo);
extern void	ipott(int fdi, int fdm, int fd_elev, int inv, int fdo,
		      float lapse, float T0, int linear);

#endif
