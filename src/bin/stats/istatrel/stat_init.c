/*
** NAME
**      stat_init -- calculate statistics on original data
**
** DESCRIPTION
**      See pgm.h for a description of values and references.
**
**      This is mostly randomness checking and some beginning 
**      info.
**
*/
#include "ipw.h"
#include "pgm.h"

void
stat_init(
	double	*arx,
	double	*ary)
{
  register int     i;
           double  sigmax  = 0,
                   sigmax2 = 0,
                   sigmay  = 0,
                   sigmay2 = 0,
                   sigmaxy = 0,
                   r;
           int     n = parm.nval;

  gstat.ymin = ary[0];
  gstat.ymax = ary[0];
  gstat.xmin = arx[0];
  gstat.xmax = arx[0];
  gstat.ssto = 0;

  for (i = 0; i < n; i++) {
    sigmax     += arx[i];
    sigmax2    += arx[i] * arx[i];
    sigmay     += ary[i];
    sigmay2    += ary[i] * ary[i];
    sigmaxy    += arx[i] * ary[i];
    if (ary[i] < gstat.ymin)  gstat.ymin = ary[i];
    if (ary[i] > gstat.ymax)  gstat.ymax = ary[i];
    if (arx[i] < gstat.xmin)  gstat.xmin = arx[i];
    if (arx[i] > gstat.xmax)  gstat.xmax = arx[i];
  }
  gstat.mean     = sigmax / n;

  r =   ( (n * sigmaxy) - (sigmax * sigmay) )
      / sqrt(   ( (n * sigmax2) - (sigmax * sigmax) )
              * ( (n * sigmay2) - (sigmay * sigmay) ) );

  for (i = 0; i < n; i++)
    gstat.ssto += ( ary[i] - gstat.mean ) * ( ary[i] - gstat.mean );

        /* put variables in global stat variable */

  gstat.lr2      = r * r;
  gstat.s2      = (sigmay2 / n ) - ( (sigmay / n) * (sigmay / n) );
  gstat.sdev    = gstat.s2 < 0 ? 0     : sqrt( gstat.s2 );
  gstat.cofv    = sigmay == 0  ? FLT_MAX : gstat.sdev / ( sigmay / n );
}
