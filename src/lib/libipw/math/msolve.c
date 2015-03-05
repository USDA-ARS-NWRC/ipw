#include <math.h>

#include "ipw.h"

static void gelim(double **A, double *b, long n);
static void backsub(double **A, double *x, double *b, long n);

/* ------------------------------------------------------------------------ */

void
msolve(
	double **A,	/* n-by-n matrix of coefficients    */
	double  *x,	/* output: answers for n variables  */
	double  *b,	/* n-by-1 matrix of constant values */
	long     n)	/* # of linear equations            */
{
  gelim(A, b, n);
  backsub(A, x, b, n);
}


/* ------------------------------------------------------------------------ */

/*
 *  Gaussian elimination, with partial pivoting.
 */

void
gelim(
	double **A,
	double  *b,
	long     n)
{
  register double   m;
  register int	    i, j, k, max;

  for (j = 0;  j < (n - 1);  j++) {
    /* partial pivot */
    max = j;
    for (k = j+1; k < n; k++) 
      if (fabs(A[k][j]) > fabs(A[max][j])) max = k;
    for (k = j; k < n; k++) {
               m = A[j][k];
      A [j]  [k] = A [max][k];
      A [max][k] = m;
    }
    m = b[j]; b[j] = b[max]; b[max] = m;
    /* end pivot */
    for (i = j + 1;  i < n;  i++) {
      m = A[i][j] / A[j][j];
      A[i][j] = 0;
      for (k = j + 1;  k < n;  k++) {
        A[i][k] -= m * A[j][k];
      }
      b[i] -= m * b[j];
    }
  }
}

/* ------------------------------------------------------------------------ */

/*
 *  Back-substitution.
 */

void
backsub(
	double **A,
	double  *x,
	double  *b,
	long     n)
{
  register double   t;
  register int	    j, k;

  for (j = (n - 1); j >= 0; j--) {
    t = 0.0;
    for (k = j + 1; k < n; k++)
      t += A[j][k] * x[k];
    x[j] = ( b[j] - t) / A[j][j];
  }
}
