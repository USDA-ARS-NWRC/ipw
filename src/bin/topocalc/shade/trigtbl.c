/*
 * trigonometric look-up table
 */

#include <math.h>
#include "ipw.h"
#include "fpio.h"

void
trigtbl(
		int             fdi,		/* input file descriptor	 */
		double          phi,		/* solar illumination aziumth	 */
		fpixel_t       *sintbl,		/* all possible values of sinS	 */
		float          *costbl,		/* all possible values of cosS	 */
		float          *cosdtbl)	/* all values of cos(phi-A)	 */
{
	FREG_1 double   x;		/* sinS or phi			 */
	REG_1 unsigned  j;		/* loop counter and B.F.	 */
	REG_2 float    *fp1;
	REG_3 fpixel_t *bfp;
	REG_4 float    *fp2;
	fpixel_t      **map;
	int            *maplen;

	map = fpmap(fdi);
	maplen = fpmaplen(fdi);

	/*
	 * sines and cosines of each possible slope
	 */

	j = maplen[0] - 1;
	fp2 = &costbl[j];
	++j;
	do {
		--j;
		x = sintbl[j];
		*fp2-- = sqrt((1 - x) * (1 + x));
	} while (j != 0);

	/*
	 * cosine differences for each possible azimuth
	 */

	j = maplen[1] - 1;
	fp1 = &cosdtbl[j];
	bfp = map[1];
	x = phi;
	++j;
	do {
		--j;
		*fp1-- = cos(x - bfp[j]);
	} while (j != 0);
}
