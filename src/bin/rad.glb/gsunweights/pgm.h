#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"

 /*
  * functions used
  */
extern void	read_headers(int fdg, int fdm, int *nlines, int *nsamps,
			     double *blat, double *blon, double *dlat,
			     double *dlon);
extern void	write_headers(int fdo, int *nbits, double *mins, double *maxs);
extern void     sunweights(int fdm, char *prefix, int kt, int year, int month,
			   int day, int nsamps, int nlines, int *nbiits,
			   double blat, double blon, double dlat, double dlon);
extern void	output(char *tempfile, int nbands, int fdo);
extern void	out_image(char *tmpname, int ikt, char *prefix, int nlines,
			  int nsamps, int *nbits, double *mins, double *maxs);

 /*
  * constants
  */
#define MAX_KT	21
#define DFLT_KT	15

#define OBANDS	3

#define MIN_AZM	-180
#define MAX_AZM	 180
#define MIN_CZ	-1
#define MAX_CZ	 1
#define MIN_WT	 0
#define MAX_WT	 1

#endif
