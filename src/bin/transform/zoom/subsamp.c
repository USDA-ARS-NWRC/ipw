#include "ipw.h"

#include "pgm.h"

/*
 * subsamp -- skip pixels in current image line, in-place
 */

void
subsamp(void)
{
	REG_4 int       dpixel;		/* # pixels to skip		 */
	REG_1 pixel_t  *i_p;		/* -> current input pixel	 */
	REG_5 int       nbands;		/* # image bands		 */
	REG_2 pixel_t  *o_p;		/* -> current output pixel	 */
	REG_6 int       samp;		/* sample counter		 */

	i_p = parm.buf;
	o_p = i_p;

	nbands = parm.nbands;
	dpixel = (parm.skip_samps - 1) * nbands;
 /*
  * loop through input samples
  */
	samp = parm.o_nsamps;
	do {
		REG_3 int       band;	/* band counter			 */

 /*
  * loop through bands
  */
		band = nbands;
		do {
			*o_p++ = *i_p++;
		} while (--band > 0);
 /*
  * skip unwanted samples
  */
		i_p += dpixel;
	} while (--samp > 0);
}
