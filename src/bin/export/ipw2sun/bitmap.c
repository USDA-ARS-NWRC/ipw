#include "ipw.h"

#include "pgm.h"

/*
 * bitmap -- convert image line to bitmap
 */

void
bitmap(
	REG_1 char     *in,		/* -> image line		 */
	REG_3 char     *out,		/* -> bitmap vector		 */
	REG_5 int       nsamps)		/* # samples / line		 */
{
	REG_2 int       out_bit;	/* current output bit		 */
	REG_4 int       out_byte;	/* current output byte		 */

	out_bit = START_BIT;
	out_byte = 0;

	do {
		if (is_set(*in++)) {
			out_byte |= out_bit;
		}

		NEXT_BIT(out_bit);
		if (out_bit == 0) {
			*out++ = out_byte;

			out_bit = START_BIT;
			out_byte = 0;
		}
	} while (--nsamps > 0);

	if (out_bit != START_BIT) {
		*out = out_byte;
	}
}
