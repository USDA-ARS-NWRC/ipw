#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

#define DFLT_NBITS	8		/* default # bits/pixel		*/

extern void iimport(FILE *ifp, int fdo, int nlines, int nsamps,
		    int nbands);
extern void quick_import(FILE *ifp, int fdo, int nlines, int nsamps,
			 int nbands);
extern void headers(int fdo, int nlines, int nsamps, int nbands,
		    int *nbits);
extern void newlqh(int fdo, int nbands, float *min, float *max);
extern void output(char *tempfile, int fdo, int nbands);
extern int  ffscanf(FILE *file, int num, float *flar);

#endif

