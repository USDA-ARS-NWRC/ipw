/*
** compute slopes
**
**	Note that tan S = sqrt(dx*dx + dy*dy)
**	If (tanS == a)
**		sinS = sqrt(a/(1+a));
*/

#include <math.h>
#include "ipw.h"
#include "fpio.h"

void
cslope(
	REG_4 int       n,		/* length of vectors	 */
	REG_1 fpixel_t *dx,		/* vector of df/dx	 */
	REG_2 fpixel_t *dy,		/* vector of df/dy	 */
	REG_3 fpixel_t *s)		/* vector of sinS	 */
{
	FREG_1 fpixel_t a;

	while (--n >= 0) {
		if (*dx != 0 || *dy != 0) {
			a = *dx * *dx + *dy * *dy;
			*s++ = sqrt(a / (1 + a));
		}
		else {
			*s++ = 0;
		}
		++dx;
		++dy;
	}
}
