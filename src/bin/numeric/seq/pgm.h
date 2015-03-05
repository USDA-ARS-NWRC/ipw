#ifndef PGM_H
#define PGM_H
 
#include "IPWmacros.h"

extern void	seqloop(double start, double fun, double inc,
			double (*inv)(double x), const char *fmt);
extern double	square(double x);

#endif
