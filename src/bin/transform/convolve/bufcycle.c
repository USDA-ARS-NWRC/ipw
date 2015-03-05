#include "ipw.h"

#include "pgm.h"

/*
 * bufcycle -- cycle pointers to input buffers
 */

void
bufcycle(
	REG_1 pixel_t **ibufs,		/* -> input line buffer array	 */
	REG_2 int       nbufs)		/* # buffers in ibufs		 */
{
	REG_3 pixel_t  *buf0;		/* -> 0th buffer in ibufs	 */

	buf0 = *ibufs;

	while (--nbufs > 0) {
		ibufs[0] = ibufs[1];
		++ibufs;
	}

	ibufs[0] = buf0;
}
