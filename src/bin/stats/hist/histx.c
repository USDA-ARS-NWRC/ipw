#include "ipw.h"

#include "pgm.h"

/*
 * histx -- accumulate histogram of 1 image line
 */

void
histx(
	REG_1 pixel_t  *i_buf,		/* -> image line		 */
	REG_3 pixel_t  *m_buf,		/* -> mask line			 */
	REG_6 int       nsamps,		/* # samples / line		 */
	REG_5 int       nbands,		/* # bands/ sample		 */
	REG_2 hist_t   **histo)		/* -> histogram array		 */
{
	do {
		REG_4 int       band;	/* band counter			 */

		band = 0;
		do {
			if (m_buf == NULL || *m_buf++ != 0) {
				++histo[*i_buf++][band];
			}
		} while (++band < nbands);
	} while (--nsamps > 0);
}
