/*
 *  NAME
 *	load_table -- load n-dimensional interpolation table from file
 *
 *  SYNPOSIS
 *	#include "pgm.h"
 *
 *	void
 *	load_table(
 *		char	   *file)	|* name of text file with table	*|
 *
 *  DESCRIPTION
 *	This function loads a n-dimensional interpolation table from the
 *	specified text file.  See the comments in the file "main.c" for
 *	a description of the format of this file.
 *
 *  RETURN VALUE
 *	None.
 *
 *  DIAGNOSTICS
 *	cannot access table file: <file>
 *		The program cannot open the table file called <file> for
 *		reading; perhaps, it doesn't exist, or permissions do not
 *		allow for read access
 *
 */

#include "ipw.h"
#include "pgm.h"

void
load_table(
	char	   *file)	/* name of text file with table	*/
{
	FILE       *tableFile;	/* -> table file 		             */
	DIM_INFO_T *dim;	/* -> current dimension record 		     */
	float      *val;	/* -> current value just read for dimension  */
	float      *prev;	/* -> previous value read for dimension      */
	int	    i;		/* loop counter				     */
	int	    j;		/* loop counter				     */


	assert(file != NULL);

	if (STREQ(file, "-")) {
	    tableFile = stdin;
	} else {
	    tableFile = fopen(file, "r");
	    if (tableFile == NULL) {
		error("cannot access table file: %s", file);
	    }
	}

	/*
	 *  Read the number of dimensions in table.
	 */
	if (fscanf(tableFile, "%d", &NumDimensions) != 1) {
	    error("cannot read # of dimensions from table file");
	}
	if (NumDimensions < 1) {
	    error("# of dimensions (= %d) in table file < 1", NumDimensions);
	}

	/* 
	 *  Allocate the dimension-info array, and loop reading in the
	 *  size and values for each dimension.
	 */
	DimensionInfo = (DIM_INFO_T *) ecalloc(NumDimensions,
					       sizeof(DIM_INFO_T));
	if (DimensionInfo == NULL) {
	    error("insufficient memory to load table");
	}
	for (i = 0, dim = DimensionInfo; i < NumDimensions; i++, dim++)
	    {
	    if (fscanf(tableFile, "%d", &(dim->size)) != 1) {
		error("cannot read size for dimension %d from table file", i+1);
	    }
	    if (dim->size < 2) {
		error("size of dimension %d (= %d) in table file < 2", i+1,
		      dim->size);
	    }

	    dim->values = (float *) ecalloc(dim->size, sizeof(float));
	    if (dim->values == NULL) {
		error("insufficient memory to load table");
	    }
	    prev = NULL;
	    for (j = 0, val = dim->values; j < dim->size; j++, val++)
		{
		if (fscanf(tableFile, "%f", val) != 1) {
		    error("cannot read value for dimension %d from table file",
			  i+1);
		}
		/*
		 *  Make sure list of values is in increasing order.
		 */
		if ((prev != NULL) && (*prev > *val)) {
		    error("values for dimension %d are not in increasing order",
			  i+1);
		}
		prev = val;
	    }  /* for each value */
	}  /* for each dimension */


	/* 
	 *  Allocate the table-sizes array, and fill it in.
	 *  Note:  start with the least significant dimension (the last
	 *         dimension), and work our ways forward in the dimension
	 *	   array.
	 */
	TableSizes = (int *) ecalloc(NumDimensions + 1, sizeof(int));
	if (TableSizes == NULL) {
	    error("insufficient memory to load table");
	}

	TableSizes[0] = 1;
	for (i = 1, dim = DimensionInfo + (NumDimensions - 1);
			i <= NumDimensions; i++, dim--)
	    {
	    TableSizes[i] = TableSizes[i-1] * dim->size;
	}


	/* 
	 *  Allocate the actual table, and read in its values.
	 */
	Table = (float *) ecalloc(TableSizes[NumDimensions], sizeof(float));
	if (Table == NULL) {
	    error("insufficient memory to load table");
	}

	for (i = 0, val = Table; i < TableSizes[NumDimensions]; i++, val++) {
	    if (fscanf(tableFile, "%f", val) != 1) {
		error("cannot read value from table file");
	    }
	}  /* for */
}
