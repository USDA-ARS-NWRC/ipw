#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "envphys.h"

/*
**	Solar data are from either
**		Thekaekara, NASA TR-R-351, 1979
**	   or
**		Makarova and Kharitinov, NASA TT F-803
**	depending on whether
**		#define THEKAEKARA
**	or	#define MAKAROVA
**	is specified below
**
**	These are adjusted to whatever value is #defined as
**	solar constant
*/

/*
** change this statement to #define MAKAROVA if Makarova-Kharitinov
** values desired
*/
#define THEKAEKARA

#ifdef THEKAEKARA
#define NSOL	172
#endif

#ifdef MAKAROVA
#define NSOL	168
#endif

extern double solar_data[][NSOL];

extern int    modprod(int a, int b, int i);
extern void   solar(int n, double *range, int *date, bool_t abbrev);
extern double solint(double a, double b);
extern double solwnt(int a, int b);
extern double solval(double lambda);
extern double solwn(int eta);
extern double spval1(double u, double x[], double y[], int n, double c[]);
extern int    updbl(double *x, double *y);
extern int    downdbl(double *x, double *y);
extern int    vdcps(int n, int *ipf, int *iexp, int *ipwr);
extern void   vtdbl(double *zot, int m, int n);
extern void   sqdbl(double *zot, int n);

#endif
