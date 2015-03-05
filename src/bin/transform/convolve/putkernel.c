#include "ipw.h"

#include "pgm.h"

/*
 * putkernel -- print kernel on stderr, for debugging
 */

void
putkernel(
	fpixel_t      **kernel,		/* -> kernel array		 */
	int             nrows,		/* # kernel rows		 */
	int             ncols)		/* # kernel columns		 */
{
	int             row;
	int             col;

	for (row = 0; row < nrows; ++row) {
		if (kernel[row] == NULL) {
			(void) fprintf(stderr, "(all 0s)\n");
		}
		else {
			for (col = 0; col < ncols; ++col) {
				(void) fprintf(stderr, "%*.*f ",
					FLT_DIG + 3, FLT_DIG,
					kernel[row][col]);
			}

			(void) fprintf(stderr, "\n");
		}
	}
}
