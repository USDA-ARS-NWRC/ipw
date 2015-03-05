#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	DFLT_CONST	0.0		/* default replacement value	 */

extern void     edimg(int i_fd, int c_fd, bool_t raw, double k, int o_fd);
extern void     getedit(int c_fd, int nlines, int nsamps, double k,
			int *esamp, int *eline, double *repl);

#endif
