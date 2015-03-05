/*
** fpixel_ting point partial derivatives in x and y directions
*/

#include "ipw.h"
#include "fpio.h"

void
diffxy(
	REG_1 int       n,		/* # samps			 */
	fpixel_t       *delh,		/* grid spacing (1-line 2-samp)	 */
	REG_3 fpixel_t *k0,		/* top line of kernel		 */
	REG_2 fpixel_t *k1,		/* middle line of kernel	 */
	REG_4 fpixel_t *k2,		/* last line of kernel		 */
	fpixel_t       *dx,		/* partial f / partial x	 */
	fpixel_t       *dy)		/* partial f / partial y	 */
{
	FREG_1 fpixel_t xfactor;	/* 1 / (2 delta h)		 */
	FREG_2 fpixel_t yfactor;	/* 1 / (2 delta h)		 */

 /*
  * divide by 2 * delta h
  */
	xfactor = 1.0 / (2.0 * delh[1]);
	yfactor = 1.0 / (2.0 * delh[0]);
 /*
  * loop
  */
	while (--n >= 0) {
		*dx++ = ((*k2++) - (*k0++)) * xfactor;
		*dy++ = ((k1[1]) - (k1[-1])) * yfactor;
		++k1;
	}
}
