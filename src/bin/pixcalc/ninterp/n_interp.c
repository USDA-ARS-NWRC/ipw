/*
 *  NAME
 *	n_interp - linearly interpolate in a n-dimensional table
 *
 *  SYNPOSIS
 *	#include "pgm.h"
 *
 *	float
 *	n_interp(
 *		int	    n_dims,	|* # of dimensions		      *|
 *		DIM_INFO_T *dim_info,	|* information about each dimension   *|
 *		float      *table,	|* actual n-dimensional table	      *|
 *		fpixel_t   *tuple)	|* n-dimensional tuple to interpolate *|
 *
 *  DESCRIPTION
 *	This function interpolates in a n-dimensional table to determine
 *	a value for a n-dimensional tuple, i.e, <x_1, x_2, ..., x_n>.
 *
 *  RETURN VALUE
 *	The interpolated value from the n-dimensional table for the given
 *	tuple.
 *
 *	If the function prints one of these error messages listed in the
 *	DIAGNOSTICS section, it does not return.
 *
 *  DIAGNOSTICS
 *	dimension n : value (...) less than minimum (...)
 *		For dimension # n, its value in the given tuple is
 *		less than the minimum value for that dimension.
 *
 *	dimension n : value (...) greater than maximum (...)
 *		For dimension # n, its value in the given tuple is
 *		greater than the maximum value for that dimension.
 */

#include "ipw.h"
#include "pgm.h"

float
n_interp(
	int	    n_dims,	/* # of dimensions		      */
	DIM_INFO_T *dim_info,	/* information about each dimension   */
	float      *table,	/* actual n-dimensional table	      */
	fpixel_t   *tuple)	/* n-dimensional tuple to interpolate */
{
	float       x;		/* value of left dimension in tuple          */
	float       x_A;	/* nearest value less than x in dimension    */
	int	    index_A;	/* index of x_A in dimension's values array  */
	float      *table_A;	/* n-1 dimensional table associated with x_A */
	float       y_A;	/* interpolated value associated with x_A    */

	int	    mid_index;	/* index in middle of current search range   */

	float       x_B;	/* nearest value greater than x in dimension */
	int	    index_B;	/* index of x_B in dimension's values array  */
	float      *table_B;	/* n-1 dimensional table associated with x_B */
	float       y_B;	/* interpolated value associated with x_B    */


	assert(n_dims > 0);

	/*
	 *  Fetch the left-most value of the tuple.
	 */
	x = tuple[0];

	/*
	 *  Lookup this value (x) in the array of values for this dimension.
	 *  Objective: determine the index of the two values bounding this
	 *	       value, i.e., the values below (x_A) and above it (x_B)
	 *
	 *  Starting with the whole array of values for this dimension,
	 *  perform a binary search.
	 */
	index_A = 0;
	index_B = dim_info->size - 1;

	/* 
	 *  Make sure the value we're looking for is in the range of values.
	 */
	if (x < dim_info->values[index_A]) {
		error("dimension %d : value (%f) less than minimum (%f)\n",
		      n_dims, x, dim_info->values[index_A]);
	}
	if (dim_info->values[index_B] < x) {
		error("dimension %d : value (%f) greater than minimum (%f)\n",
		      n_dims, x, dim_info->values[index_B]);
	}

	while (index_A + 1 < index_B) {
		mid_index = (index_A + index_B) / 2;
		if (x < dim_info->values[mid_index]) {
			index_B = mid_index;
		} else if (dim_info->values[mid_index] < x) {
			index_A = mid_index;
		} else {
			index_A = index_B = mid_index;
		}
	}  /* while */

	x_A = dim_info->values[index_A];

	/*
	 *  Determine the interpolated value (y_A) associated with x_A.
	 */
	if (n_dims > 1) {
		/*
		 *  The given table is more than 1 dimensions, so recurse
		 *  on the sub-table of n-1 dimensions associated with
		 *  with x_A.
		 */
		table_A = table + (index_A * TableSizes[n_dims - 1]);
		y_A = n_interp(n_dims - 1, dim_info + 1, table_A, tuple + 1);
	} else {
		/*
		 *  One dimension table, so y_A is just the value at
		 *  index_A.
		 */
		y_A = table[index_A];
	}

	/*
	 *  If x was found in the array of values (i.e., x = x_A = x_B), then
	 *  no need to interpolate for this dimension; we're done.
	 */
	if (index_A == index_B) {
		return y_A;
	}

	/*
	 *  Determine the interpolated value (y_B) associated with x_B
	 *  in the same manner as above for y_A and x_A.
	 */
	x_B = dim_info->values[index_B];
	if (n_dims > 1) {
		/*
		 *  The given table is more than 1 dimensions, so recurse
		 *  on the sub-table of n-1 dimensions associated with
		 *  with x_B.
		 */
		table_B = table + (index_B * TableSizes[n_dims - 1]);
		y_B = n_interp(n_dims - 1, dim_info + 1, table_B, tuple + 1);
	} else {
		/*
		 *  One dimension table, so y_B is just the value at
		 *  index_B.
		 */
		y_B = table[index_B];
	}

	/*
	 *  Linearly interpolate for the value (y) associated with x.
	 */
	return ((y_B - y_A) / (x_B - x_A)) * (x - x_A) + y_A;
}
