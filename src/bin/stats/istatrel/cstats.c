/*
** NAME
**      cstats -- calculate statistics
**
** DESCRIPTION
**      See pgm.h for a list of values calculated and references
**
*/
#include "ipw.h"
#include "pgm.h"

void
cstats(
	double  * arx,
	double  * ary,
	long      d,
	double  * x,
	STAT_T  * stat)
{
  int     i, j;
  double  tx, ty, res;
  double  sigmay  = 0,
          sigmay2 = 0;
  double  sse = 0,
          chi2 = 0;
  int     n = parm.nval;

  stat->ymin =  1E38;
  stat->ymax = -1E38;

  for (i = 0; i < n; i++) {
    tx = arx[i];
    ty = x[d];
    for (j = d-1; j >= 0; j--)
      ty = tx * ty + x[j];  

    res       = ary[i] - ty;
    sse      += res * res;
    sigmay   += ty;
    sigmay2  += ty * ty;
    chi2     += res * res / ( ty < 0 ? -ty : ty );
    if (ty < stat->ymin)  stat->ymin = ty;
    if (ty > stat->ymax)  stat->ymax = ty;
  }

        /* put variables in global stat variable */

  d++;
  stat->o2      = (sigmay2 / n ) - ( (sigmay / n) * (sigmay / n) );
  stat->odev    = stat->o2 < 0  ? 0       : sqrt( stat->o2 );
  stat->cofv    = sigmay == 0  ? FLT_MAX : stat->odev / ( sigmay / n );
  stat->chi2    = chi2;
  stat->cofc    = sqrt( stat->chi2 / ( stat->chi2 + n) );
  stat->sse     = sse;
  stat->ssr     = gstat.ssto - stat->sse;
  stat->msr     = d == 1       ? FLT_MAX : stat->ssr / (d - 1);
  stat->mse     = d == n       ? FLT_MAX : stat->sse / ( n - d );
  stat->fstar   = stat->msr / stat->mse;
  stat->r2      = d == n       ? FLT_MAX : 1.0 - ( (n-1) / (n-d) )
                                                * (stat->sse / gstat.ssto);
  stat->syx     = sqrt( stat->sse / n );
}
