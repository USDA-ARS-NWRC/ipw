#include "ipw.h"
#include "mvalloc.h"

#if defined(__GNUC__) && defined(__STDC__) && !defined(__STRICT_ANSI__)
#define INLINE inline
#else
#define INLINE
#endif

static INLINE double fitf(double a, int x);

void
apfit(
	double    *arx,	   /* array of x values			       */
	double    *ary,	   /* array of y values			       */
	long       n,	   /* length of arrays  		       */
	long       d,	   /* length of output array c		       */
	double    *c,	   /* output: array of polynomial coefficients */
	const int savemem) /* use less memory and run slower?	       */
{
  register int        k, j, i;
  register double     tv;
           double  ** A;
           double  ** m;
           double  ** tm;
           double   * b;

  d++;

  A  = (double **) MValloc(MV_DOUBLE, 2, d, d);     /* make arrays      */
  b  = (double *) MValloc(MV_DOUBLE, 1, d);
  if (!savemem) {
    m  = (double **) MValloc(MV_DOUBLE, 2, n, d);
    tm = (double **) MValloc(MV_DOUBLE, 2, d, n);
  } else {
    m  = (double **) NULL;    /* To make lint and gcc stop complaining  */
    tm = (double **) NULL;    /* about variables used but not set.      */
  }

  for (i = 0; i < d; i++) {             /* zero arrays      */
    b[i] = 0;
    for (j = 0; j < d; j++)
      A[i][j] = 0;
  }

  if (!savemem) {
    for (i = 0; i < n; i++)               /* initialize m     */
      for (j = 0; j < d; j++) {
              tv = fitf(arx[i], j);
         m[i][j] = tv;                    /* set m            */
        tm[j][i] = tv;                    /* set transpose m  */
      }
  }

  if (!savemem) {
    for (i=0; i < d; i++)                 /* b = tm * ary     */
        for (k = 0; k < n; k++)
          b[i] += tm[i][k] * ary[k];
  } else {
    for (i=0; i < d; i++)                 /* b = tm * ary     */
        for (k = 0; k < n; k++)
          b[i] += fitf(arx[k], i) * ary[k];
  }


  if (!savemem) {
    for (i=0; i < d; i++)                 /* A = tm * m       */
      for (j=0; j < d; j++)
        for (k = 0; k < n; k++)
          A[i][j] += tm[i][k] * m[k][j];
  } else {
    for (i=0; i < d; i++)                 /* A = tm * m       */
      for (j=0; j < d; j++)
        for (k = 0; k < n; k++)
          A[i][j] += fitf(arx[k], i) * fitf(arx[k], j);
  }


  msolve(A, c, b, d);                   /* solve   A x = b   */

  MVfree((void *) A );
  MVfree((void *) b );
  if (!savemem) {
    MVfree((void *) m );
    MVfree((void *) tm);
  }
}

/* ------------------------------------------------------------------------ */

/*
 *   The fitting function:  a^x
 */

static INLINE double
fitf(
	double a,
	int    x)
{
  double v = 1;

  while (x-- > 0)
    v *= a;
  return(v);
}
