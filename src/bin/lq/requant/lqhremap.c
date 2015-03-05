/*
** NAME
**      lqhremap -- remap a lqh header for a band
**
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "fpio.h"
#include "pgm.h"

int
lqhremap(LQH_T *lqhp, int oldbits, int newbits)
{
  int        bp;
  int        pval;
  int        pval_old;
  int        oldmaplen;
  int        newmaplen;
  int       *oldbkpt;
  int       *newbkpt;
  fpixel_t  *oldmap;
  fpixel_t  *newmap;
  double     scale;

  if (oldbits == newbits)
    return(OK);

  if (lqhp == NULL)
    return(OK);

  assert( (oldbits > 0) && (oldbits <= sizeof(pixel_t) * CHAR_BIT) );
  assert( (newbits > 0) && (newbits <= sizeof(pixel_t) * CHAR_BIT) );


  oldmaplen = lqh_maplen(lqhp);
  oldbkpt = lqh_bkpt(lqhp);
  oldmap = lqh_map(lqhp);


  newmaplen = ipow2(newbits);

  if (lqh_nbkpts(lqhp) > newmaplen) {
    error("%d bits does not permit %d breakpoints", newbits, lqh_nbkpts(lqhp));
    /* NOTREACHED */
    return(ERROR);
  }

  newbkpt = (int *) ecalloc(newmaplen, sizeof(int));
  if (newbkpt == NULL)
    return(ERROR);

  newmap = (fpixel_t *) ecalloc(newmaplen, sizeof(fpixel_t));
  if (newmap == NULL)
    return(ERROR);

  scale = ( (double) ipow2(newbits) - 1) / ( (double) ipow2(oldbits) - 1);


  pval_old = -1;
  for (bp = 0; bp < lqh_nbkpts(lqhp); bp++) {
    pval = oldbkpt[bp];
    pval = (int) ( scale * (double) pval + 0.5);
    /* Correct for breakpoints clustered at the end */
    if (pval + (lqh_nbkpts(lqhp) - bp)  >  newmaplen) {
      pval = newmaplen - ( lqh_nbkpts(lqhp) - bp );
    }
    /* Correct for breakpoints which map to the same value */
    if (pval <= pval_old) {
      pval = pval_old+1;
   }
    pval_old = pval;
    newbkpt[bp] = pval;
    newmap[pval] = oldmap[oldbkpt[bp]];
  }

  if (lqh_mapped(lqhp) == TRUE) {
    lqh_mapped(lqhp) = FALSE;
    SAFE_FREE(lqh_lininv(lqhp));
  }


  lqhp->maplen = newmaplen;
  lqhp->bkpt = newbkpt;
  lqhp->map = newmap;
  SAFE_FREE(oldbkpt);
  SAFE_FREE(oldmap);

  return(OK);
}
