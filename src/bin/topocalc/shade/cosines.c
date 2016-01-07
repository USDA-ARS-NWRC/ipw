/*
 * read input slope/azimuth data, compute cosine of
 * illumination angle, and write
 */

#include "ipw.h"

#include "pgm.h"

void
cosines(
		int             fdi,		/* input file desc	 */
		int             fdo)		/* output file desc	 */
{
	REG_1 pixel_t  *sp;		/* -> slope index	 */
	REG_2 int       j;		/* loop counter		 */
	REG_3 pixel_t  *b;		/* output value		 */
	int             ngot;		/* # pixels read	 */
	int             nwrite;		/* # pixels to write	 */

	/* default values */
	nwrite = nget;

	/*
	 * read pixels until done
	 */

	while ((ngot = pvread(fdi, ibuf, nget)) > 0) {
		if (ngot < nget) {
			nwrite = ngot;
		}

		/* -> input */
		sp = ibuf;

		/* -> output */
		b = obuf;

		for (j = nwrite; --j >= 0;) {

			/*
			 * fetch value from look-up table first band is slope, second azimuth
			 * (afterward sp -> slope of next sample)
			 */

			*b++ = shade[*sp][sp[1]];
			sp += 2;
		}

		if (pvwrite(fdo, obuf, nwrite) != nwrite) {
			error("write error");
		}
	}

	if (ngot != 0) {
		error("read error");
	}
}
