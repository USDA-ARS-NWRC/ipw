#include <stdio.h>
#include <string.h>
#include "ipw.h"
#include "pgm.h"

#define ANNOT_PREFIX "Interpolated from "

void
read_par(double *poly)
{
  int    i;
  char   iannot[strlen(ANNOT_PREFIX) + PATH_MAX + 1];   /* +1 for newline */
  int	 len;

  for (i = 0; i <= parm.degree; i++)
    scanf( "%lf\n", &poly[i]);
  scanf("%lf\n%lf\n", &pstat.xmin, &pstat.xmax);
  scanf("%lf\n%lf\n", &pstat.ymin, &pstat.ymax);
  scanf("%lf\n%lf\n", &pstat.syx, &pstat.sdev);
  scanf("%lf\n%lf\n", &pstat.d_r2, &pstat.r2);
  scanf("%lf\n%lf\n", &pstat.d_cofv, &pstat.cofv);
  scanf("%lf\n", &pstat.cofc);

  strcpy(iannot, ANNOT_PREFIX);
  (void) fgets(iannot + strlen(ANNOT_PREFIX), PATH_MAX + 1, stdin);
  /*
   *  Remove newline at end of string
   */
  len = strlen(iannot);
  if (iannot[len-1] == '\n') {
    iannot[len-1] = '\0';
  }
  parm.annot = addsv (NULL, iannot);
   
  if (parm.verbose) {
    fprintf(stderr, "max degree: %ld\n", parm.degree);
    for (i = 0; i <= parm.degree; i++)
      fprintf(stderr, "p[%d]: %.19g\n", i, poly[i]);
    fprintf(stderr, "xmin: %f\nxmax: %f\n", pstat.xmin, pstat.xmax);
    fprintf(stderr, "ymin: %f\nymax: %f\n", pstat.ymin, pstat.ymax);
    fprintf(stderr, "syx:  %f\nsdev: %f\n", pstat.syx, pstat.sdev);
    fprintf(stderr, "d_r2: %f\nr2:   %f\n", pstat.d_r2, pstat.r2);
    fprintf(stderr, "d_cofv: %f\ncofv: %f\n", pstat.d_cofv, pstat.cofv);
    fprintf(stderr, "cofc: %f\n", pstat.cofc);
    fprintf(stderr, "annot: %s\n", walksv(parm.annot, TRUE));
  }

}
