#include "ipw.h"

#include "pgm.h"

/*
 * getkernel -- read convolution kernel
 */

fpixel_t      **
getkernel(
	FILE           *file,		/* kernel file 			 */
	int            *nrowsp,		/* -> # rows in kernel		 */
	int            *ncolsp)		/* -> # columns in kernel	 */
{
	REG_2 int       col;		/* current kernel column	 */
	REG_1 fpixel_t **kernel;	/* -> kernel array		 */
	FREG_1 fpixel_t ksum;		/* sum of coefficients		 */
	REG_4 int       nrows;		/* # rows in kernel		 */
	REG_5 int       ncols;		/* # columns in kernel		 */
	REG_3 int       row;		/* current kernel row		 */

 /*
  * read kernel size
  */
	if (fscanf(file, "%d %d", nrowsp, ncolsp) != 2) {
		error("can't read kernel size");
	}

	nrows = *nrowsp;
	ncols = *ncolsp;

	if (nrows < 1 || ncols < 1) {
		error("bad kernel size: %dx%d", nrows, ncols);
	}

	if (!(nrows & 1 && ncols & 1)) {
		error("both kernel dimensions must be odd");
	}
 /*
  * allocate kernel array
  */
 /* NOSTRICT */
	kernel = (fpixel_t **) allocnd(sizeof(fpixel_t), 2, nrows, ncols);
	if (kernel == NULL) {
		error("can't allocate kernel array");
	}
 /*
  * read kernel
  */
	ksum = 0.0;

	for (row = 0; row < nrows; ++row) {
		bool_t          zero;	/* ? all cols in this row == 0	 */

		zero = TRUE;

		for (col = 0; col < ncols; ++col) {
			double          k;	/* current kernel value	 */

			if (fscanf(file, "%lf", &k) != 1) {
				error("can't read kernel[%d][%d]", row, col);
			}

			if (k != 0.0) {
				kernel[row][col] = k;

				ksum += k;
				zero = FALSE;
			}
		}
 /*
  * if this row is all 0's then toss it
  */
		if (zero) {
			kernel[row] = NULL;
		}
	}

	(void) fclose(file);
 /*
  * normalize kernel so coefficients sum to 1 (unless they already sum to 0)
  */
	if (ksum != 0) {
		for (row = 0; row < nrows; ++row) {
			if (kernel[row] != NULL) {
				for (col = 0; col < ncols; ++col) {
					kernel[row][col] /= ksum;
				}
			}
		}
	}
 /*
  * done
  */
	return (kernel);
}
