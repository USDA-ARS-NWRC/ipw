#include "ipw.h"
#include "pgm.h"

void
stat_anal()
{
  double errp  = pstat.syx  / pstat.sdev;
  double cofvp = pstat.cofv / pstat.d_cofv;

  if (pstat.d_r2 > 0.9) 
    fprintf(stderr, "Good: Very high linear correlation in original data.\n");

  if (pstat.d_cofv < 0.1) 
    fprintf(stderr, "Good: Very low coeff of variation in original data.\n");

  if (pstat.r2 < 0.5)
    fprintf(stderr, "Bad:  Very low coeff of multiple determination.\n");

  if (pstat.cofc > 0.9)
    fprintf(stderr, "Bad:  Very high coeff of contingency.\n");
  else if (pstat.cofc > 0.5)
    fprintf(stderr, "Ave:  High coeff of contingency.\n");
  else if (pstat.cofc < 0.1)
    fprintf(stderr, "Good: Very low coeff of contingency.\n");

  if (errp > 0.9)
    fprintf(stderr, "Bad:  High error percentage.  Can't establish relation.\n");
  else if (errp > 0.5)
    fprintf(stderr, "Ave:  Relation has high error percentage.\n");
  else if (errp < 0.1)
    fprintf(stderr, "Good: Relation is very strong.\n");

  if (cofvp < 0.1)
    fprintf(stderr, "Bad:  High cofv percentage.  Can't establish relation.\n");
  else if (cofvp > 0.5)
    fprintf(stderr, "Good: Relation established.\n");
  else if (cofvp > 0.9)
    fprintf(stderr, "Good: Relation is very strong.\n");

}
