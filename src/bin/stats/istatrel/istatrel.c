/*
** NAME
**      istatrel -- calculate statistical relationship
**
** DESCRIPTION
**      does sanity checks on degree (max degree <= number of points),
**                                   (degree < max degree).
**      loop until at maximum degree or we satisfy tolerance level {
**        fit polynomial to data
**        calculate statistics
**        if verbose is on, print out information
**        try the next highest degree
**      }
**      print previous polynomial and statistics info
**
*/

#include "ipw.h"
#include "pgm.h"

#define FUL_FORMAT  "%.15g"
#define NUM_FORMAT  "%.6g"
#define DEG_FORMAT  " x^%ld"

void
istatrel(
	double	*ax,
	double	*ay)
{
  long      degree = parm.startd;
  long      i;
  double    p_errp = 1e38;
  double    errp   = 1e09;
  STAT_T    cstat, pstat;

        /* sanity checks */

  if (parm.maxd > parm.nval) 
    parm.maxd = parm.nval;     /* can't have a poly of order greater than n */

  if (degree >= parm.maxd)
    degree = parm.maxd - 1;    /* can't have a poly of order greater than max */


  while ( (degree < parm.maxd) && ( (p_errp - errp) > parm.tolerance ) ) {
    pstat = cstat;
    p_errp = errp;
    for (i = 0; i < parm.maxd; i++)   parm.ppoly[i] = parm.cpoly[i];
    apfit( ax, ay, parm.nval, degree, parm.cpoly, parm.space);
    cstats( ax, ay, degree, parm.cpoly, &cstat);
    errp = cstat.syx / gstat.sdev;
    if (parm.verbose) {
      fprintf(stderr, "\n\n");
      fprintf(stderr, "data:   lr2  : %7f   cofv : %7f   sdev : %7f\n",
             gstat.lr2, gstat.cofv, gstat.sdev);
      fprintf(stderr, "est :   r2   : %7f   cofc : %7f   syx  : %7f\n",
             cstat.r2, cstat.cofc, cstat.syx);
      fprintf(stderr, "est%%:   err%% : %7f   %%cofv: %7f\n",
             cstat.syx / gstat.sdev, cstat.cofv / gstat.cofv);
    }
    if (parm.show || parm.verbose) {
      fprintf(stderr, "order %ld: ", degree);
      fprintf(stderr, NUM_FORMAT, parm.cpoly[0] );
      for (i = 1; i <= degree; i++)
        if ( fabs(parm.cpoly[i]) > 1.0e-99 ) {
          fprintf(stderr, " + ");
          fprintf(stderr, NUM_FORMAT, parm.cpoly[i]);
          fprintf(stderr, DEG_FORMAT, i);
        }
      fprintf(stderr, " (%.6f)\n", errp);
    }
    degree++;
  }

  if (degree == parm.maxd ) {
    degree--;
    pstat = cstat;
    for (i = 0; i < parm.maxd; i++)   parm.ppoly[i] = parm.cpoly[i];
  } else {
    degree -= 2;
  }

  printf("%d\n", parm.nfiles-1);
  printf("%ld\n", degree);
  for (i = 0; i <= degree; i++) {
    printf(FUL_FORMAT, parm.ppoly[i]);
    printf("\n");
  }
  printf("%f\n%f\n", gstat.xmin, gstat.xmax);
  printf("%f\n%f\n", pstat.ymin, pstat.ymax);
  printf("%f\n%f\n", pstat.syx, gstat.sdev);
  printf("%f\n%f\n", gstat.lr2, pstat.r2);
  printf("%f\n%f\n", gstat.cofv, pstat.cofv);
  printf("%f\n", pstat.cofc);
  printf("%s\n", walksv(parm.annot, TRUE));
}
