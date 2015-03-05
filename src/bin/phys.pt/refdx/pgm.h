#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "ipw.h"

extern int		akcoef(double *x, double *y, int nx, double *c);

extern COMPLEX_T *	refice(COMPLEX_T *result, double lambda,
			       double temp);
extern COMPLEX_T *	trefice(COMPLEX_T *result, double lambda,
				double temp);
extern COMPLEX_T *	refh2o(COMPLEX_T *result, double lambda,
			       double temp);
extern COMPLEX_T *	trefh2o(COMPLEX_T *refwat, double lambda,
				double tc);
extern int		modprod(int a, int b, int i);
extern double 		spval1(double u, double x[], double y[], int n,
			       double c[]);
extern int		vdcps(int n, int *ipf, int *iexp, int *ipwr);
extern void		vtdbl(double *zot, int m, int n);
extern void		sqdbl(double *zot, int n);
extern void		wavechoice(char *units, double (**xform)(double));
extern double		multiply(double x);
extern double		divide(double x);

#endif
