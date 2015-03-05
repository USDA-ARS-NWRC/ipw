#include "ipw.h"

#include "pgm.h"

/*
 * conv1d -- 1-dimensional convolution
 */

void
conv1d(
	REG_3 pixel_t  *ibuf,		/* -> input line		 */
	int             nsamps,		/* # samples / line		 */
	REG_2 fpixel_t **kernel,	/* -> convolution kernel maps	 */
	REG_5 int       ncols,		/* # kernel columns		 */
	REG_4 fpixel_t *obuf)		/* -> output line		 */
{
 /*
  * make obuf -> first convolvable sample
  */
	obuf += ncols / 2;
 /*
  * loop through convolvable samples
  */
	nsamps -= ncols - 1;
	do {
		FREG_1 fpixel_t sum;	/* fast FP accumulator		 */
		REG_6 int       col;	/* kernel column counter	 */

		sum = *obuf;
 /*
  * loop through kernel columns
  */
		col = ncols;
		do {
			REG_1 fpixel_t *kp;	/* fast -> kernel map	 */

 /*
  * if current kernel element is non-0, then accumulated weighted value of
  * current input pixel
  */
			kp = *kernel++;
			if (kp != NULL) {
				sum += kp[*ibuf];
			}

			++ibuf;
		} while (--col > 0);

		*obuf++ = sum;

		ibuf -= ncols;
		++ibuf;

		kernel -= ncols;
	} while (--nsamps > 0);
}
