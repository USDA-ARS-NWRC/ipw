/*
** fill fpixel vector with constants
*/

#include "ipw.h"

void
fillvec(
	REG_1 fpixel_t *x,		/* vector to fill	 */
	FREG_1 fpixel_t fill,		/* fill value		 */
	REG_2 int       n)		/* # elements		 */
{
	while (--n >= 0) {
		*x++ = fill;
	}
}
