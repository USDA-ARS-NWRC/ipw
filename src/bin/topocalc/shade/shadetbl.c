/*
 * store all possible illumination cosines
 */

#include "ipw.h"
#include "fpio.h"

void
shadetbl(
		int             fdi,		/* input file descriptor	 */
		int             fdo,		/* output file descriptor	 */
		FREG_2 float    ctheta,		/* cosine zenith angle		 */
		FREG_3 float    stheta,		/* sine zenith angle		 */
		REG_4 float    *costbl,		/* all possible values of cosS	 */
		REG_3 float    *sintbl,		/* all possible values of sinS	 */
		REG_5 float    *cosdtbl,	/* all values of cos(phi-A)	 */
		REG_6 pixel_t **shade)		/* lookup table			 */
{
	REG_1 unsigned  a;		/* azimuth index		 */
	REG_2 unsigned  s;		/* slope index			 */
	FREG_1 float    mu;		/* cosine local illum angle	 */
	REG_3 int       maxval;		/* maximum integer output value	 */
	int            *imaplen;	/* lengths of input maps	 */
	int            *omaplen;	/* lengths of output map	 */

	imaplen = fpmaplen(fdi);
	omaplen = fpmaplen(fdo);

	/*
	 * loop through every possible slope and azimuth; the loops look
	 * strange because we use unsigned loop counters
	 */

	maxval = omaplen[0] - 1;
	s = imaplen[0];
	do {
		a = imaplen[1];

		--s;

		do {
			--a;

			/* cosine of local illumination angle */
			mu = ctheta * costbl[s] +
					stheta * sintbl[s] * cosdtbl[a];
			if (mu < 0)
				mu = 0;
			else if (mu > 1)
				mu = 1;

			/* Following line changed 8/28/90 by K. Longley USEPA ERL-C:
			 *                     shade[s][a] = mu * maxval + 0.5;
			 *             Removed addition of 0.5 to prevent rounding
			 *             Fpio library would not round if we were using fpvwrite instead
			 *             of doing the mapping ourselves.
			 */
			shade[s][a] = mu * maxval;

		} while (a != 0);

	} while (s != 0);
}
