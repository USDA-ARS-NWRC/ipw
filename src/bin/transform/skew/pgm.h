#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	SCR_PREFIX	"skew"		/* scratch file name prefix	*/

extern void     skew(int i_fd, bool_t fwd, double angle,
                              bool_t skip_skewh, int o_fd);

#endif
