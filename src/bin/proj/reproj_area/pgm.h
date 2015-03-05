#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#include "mapproj.h"

extern void	headers(int fdi, double *bline, double *bsamp, double *dline,
			double *dsamp);
extern void	reproj_area(int fdi, double bline, double bsamp, double dline,
			    double dsamp, struct projdef *iproj,
			    struct projdef *oproj);

#endif
