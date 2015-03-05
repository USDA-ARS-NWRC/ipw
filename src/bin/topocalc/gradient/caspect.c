
/*
 * compute aspects
 *
 *  There is some disagreement over what exactly atan2 should return
 *  when one of it's arguments is 0.  To end the problem, we set up
 *  the special cases ourselves, and only call atan2 if both arguments
 *  are non-zero.
 *
 *  -- 4/30/93   Dana Jacobsen, ERL-C.
 */
 
#include <math.h>
#include "ipw.h"
#include "fpio.h"
 
void
caspect(
	int       n,		/* length of vectors     */
	fpixel_t *dx,		/* vector of df/dx       */
	fpixel_t *dy,		/* vector of df/dy       */
	fpixel_t *a)		/* vector of aspects     */
{
  while (--n >= 0) {
    if (*dy != 0) {
      if (*dx != 0) {
        /* dx and dy are non-zero */
        *a++ = (fpixel_t) atan2( (double) -(*dy), (double) -(*dx) );
      } else {
        /* dx is zero */
        *a++ = (*dy > 0.0)  ?  -M_PI/2  :  M_PI/2;
      }
    } else {
      if (*dx != 0) {
        /* dy is zero */
        *a++ = (*dx > 0.0)  ?  -M_PI  :  -0.0;
      } else {
        /* dx and dy are zero */
        *a++ = 0.0;
      }
    }
    ++dy;
    ++dx;
  }
}
