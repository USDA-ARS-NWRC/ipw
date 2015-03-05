/*
** fill ends of line 1
** fill fictitious line 0
*/
#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
fillstart(
	int             n,		/* genuine # samples	 */
	REG_3 float    *k0,		/* -> line 0		 */
	REG_1 float    *k1,		/* -> line 1		 */
	REG_4 float    *k2)		/* -> line 2		 */
{
	REG_2 int       j;		/* index		 */

 /*
  * ends of line 1
  */
	fillends(n, k1);

 /*
  * line 0
  */
	for (j = n - 1; j >= 0; --j) {
		k0[j] = k1[j] + (k1[j] - k2[j]);
	};

 /*
  * ends of line 0
  */
	fillends(n, k0);
}
