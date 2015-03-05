#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	DFLT_BLINE	0
#define	DFLT_BSAMP	0
#define	DFLT_DLINE	1
#define	DFLT_DSAMP	1

extern void	mkwinh(int fdi, double bline, double bsamp, double dline,
		       double dsamp, bool_t force, int fdo);

#endif
