/*
** shuffle slopes and aspects into merged output buffer
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
shuffle(
	REG_4 int       n,		/* vector length	 */
	REG_2 fpixel_t *s,		/* slope vector		 */
	REG_3 fpixel_t *a,		/* aspect vector	 */
	REG_1 fpixel_t *o)		/* output buffer	 */
{
	while (--n >= 0) {
		*o++ = *s++;
		*o++ = *a++;
	}
}
