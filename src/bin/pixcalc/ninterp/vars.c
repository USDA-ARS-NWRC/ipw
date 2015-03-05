/*
 *  Global variables for "ninterp" program.
 */

#include "ipw.h"
#include "pgm.h"

int NumDimensions = 0;
	/*
	 *  # of dimensions of in the interpolation table.
	 */

DIM_INFO_T *DimensionInfo = NULL;
	/*
	 *  An array of dimension records that have the size and list of
	 *  values for each dimension.
	 */

int *TableSizes = NULL;
	/*
	 *  An array of table sizes indexed by the number of dimensions.
	 *  By default, the first size (index 0) = 1.  The remaining sizes
	 *  are initialized from the individual dimensions' sizes.
	 *
	 *  For example, suppose we have 3 dimensions with these sizes:
	 *  4, 6, and 10.  So our master 3-dimensional table consists of
	 *  4 2-dimensional tables.  Each 2-dimensional table has 6
	 *  1-dimensional tables of 10 elements each.  Therefore,
	 *
 	 *	TableSizes[0] =   1
 	 *	TableSizes[1] =  10
 	 *	TableSizes[2] =  60  (= 6 * 10)
 	 *	TableSizes[3] = 240  (= 4 * 60)
	 */

float *Table = NULL;
	/*
	 *  The interpolation table itself; values are stored in what
	 *  would be 'row-major' order for a 2-dimensional table.  See
	 *  the comments in the file "main.c" for details.
	 */

IMAGE_T	*  i_image;		/* input image			 */
IMAGE_T *  o_image;		/* output image			 */

int	   num_lines;		/* # of lines in image		 */
int	   num_samples;		/* # of samples per line	 */
int	   num_bands;		/* # of bands per sample	 */
