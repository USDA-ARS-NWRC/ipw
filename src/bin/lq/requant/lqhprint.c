/*
** NAME
**      lqhprint -- print an lqh map on the standard error
**
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "fpio.h"
#include "pgm.h"

int
lqhprint(LQH_T *lqhp)
{
  int        bp;

  if (lqhp == NULL) {
    fprintf(stderr, "LQH is null.\n");
    return(OK);
  }

  fprintf(stderr, "%d breakpoints, maplength of %d",
          lqh_nbkpts(lqhp), lqh_maplen(lqhp));
  if (lqh_mapped(lqhp))
    fprintf(stderr, ", lqh is mapped.\n");
  else
    fprintf(stderr, ", lqh is not mapped.\n");
  for (bp = 0; bp < lqh_nbkpts(lqhp); bp++) {
    fprintf(stderr, "bp: %4d, pixel bkpt: %10d, fpixel map: %12f\n",
            bp, lqh_bkpt(lqhp)[bp], lqh_map(lqhp)[lqh_bkpt(lqhp)[bp]]);
  }

  return(OK);
}
