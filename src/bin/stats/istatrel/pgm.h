#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "istatrel" program
 */

typedef struct {
  int        nfiles;	/* number of files               */
  int        im1;	/* input image1 file descriptor	 */
  int        im2;	/* input image2 file descriptor	 */
  int        imask;	/* mask image file descriptor	 */
  int        nlines;    /* lines                         */
  int        nsamps;    /* samples                       */
  long       nval;      /* number of points (after mask and clip) */
  long       maxn;      /* maximum number of points      */
  long       maxd;      /* maximum degree                */
  long       startd;    /* starting degree               */
  int        doclip;	/* ? do clip                     */
  fpixel_t   clipmax;   /* maximum clipping value        */
  fpixel_t   clipmin;   /* minimum clipping value        */
  fpixel_t   tolerance; /* error % tolerance             */
  int        verbose;   /* ? verbose                     */
  int        show;      /* ? show values                 */
  int        space;     /* ? time-space tradeoff         */
  STRVEC_T * annot;     /* annotation header             */
  double   * ppoly;     /* previous polynomial           */
  double   * cpoly;     /* current polynomial            */
} PARM_T;

/*
    S == Schaum's Outline Series: Theory and Problems of Statistics
    L == Applied Linear Regression Models.  Neter, Wasserman, & Kutner
*/
typedef struct {
  double  ymin,
          ymax,
          mean,
          xmin,
          xmax;
  double  s2, 
          sdev,
          cofv,
          lr2,          /* linear correlation coeff     S 244  */
          ssto;         /* total sum of squares         L  87  */
} GSTAT_T;

typedef struct {
  double  ymin,
          ymax;
  double  o2,           /* population variance          S  70  */
          odev,         /* standard deviation           S  70  */
          cofv,         /* coefficient of variation     S  73  */
          r2;           /* coefficient of multiple det  L 241  */
  double  syx,          /* standard error of estimate   S 243  */
          chi2,         /* not really chi squared	       */
          cofc;         /* coefficient of contingency	S 204  */
  double  ssr,          /* regression sum of squares    L  89  */
          ssto,         /* total sum of squares         L  87  */
          sse,          /* error sum of squares         L  50  */
          msr,          /* regression mean square       L  91,239 */
          mse,          /* error mean square            L  50  */
          fstar;        /* test statistic               L  95  */
} STAT_T;

PARM_T   parm;
GSTAT_T  gstat;

extern void headers(void);
extern void ipwload(double *ax, double *ay);
extern void stat_init(double *arx, double *ary);
extern void istatrel(double *ax, double *ay);
extern void cstats(double *arx, double *ary, long d, double *x, STAT_T *stat);

extern double fabs(double x);
extern double sqrt(double x);

#endif
