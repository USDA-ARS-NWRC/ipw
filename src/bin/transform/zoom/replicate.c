#include "ipw.h"

#include "pgm.h"

/*
 * replicate -- replicate pixels in current image line, in-place
 */

void
replicate(void)
{
	REG_1 pixel_t  *i_p;		/* -> current input pixel	 */
	REG_4 int       nbands;		/* # image bands		 */
	REG_2 pixel_t  *o_p;		/* -> current output pixel	 */
	REG_6 int       samp;		/* sample counter		 */

	nbands = parm.nbands;

	i_p = &parm.buf[parm.i_nsamps * nbands];
	o_p = &parm.buf[parm.o_nsamps * nbands];
 /*
  * loop through input samples
  */
	samp = parm.i_nsamps;
	do {
		REG_5 int       nreps;	/* # replications		 */

 /*
  * loop for # replications
  */
		nreps = parm.dup_samps;
		for (;;) {
			REG_3 int       band;	/* band counter		 */

 /*
  * loop through bands
  */
			band = nbands;
			do {
				*--o_p = *--i_p;
			} while (--band > 0);

			if (--nreps <= 0) {
				break;
			}
 /*
  * if still replicating then repeat current input pixel
  */
			i_p += nbands;
		}
	} while (--samp > 0);
}
