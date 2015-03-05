#include "IPWmacros.h"

/*
 *  Include file for the "ninterp" program.
 */

#include "image.h"

/*
 *  # of bands in output image
 */
#define OBANDS 1

/*
 *  Record structure for a dimension.
 */
typedef struct {
	int	  size;		/* dimension's size, i.e., # of values */
	fpixel_t *values;	/* reference values in increasing order */
} DIM_INFO_T;

/* ------------------------------------------------------------------------ */

/*
 *  Global variables internal to this program.
 */

extern	int	    NumDimensions;
extern	DIM_INFO_T *DimensionInfo;
extern	int	   *TableSizes;
extern	float	   *Table;

extern	IMAGE_T	*  i_image;		/* input image			 */
extern	IMAGE_T *  o_image;		/* output image			 */

extern	int	   num_lines;		/* # of lines in image		 */
extern	int	   num_samples;		/* # of samples per line	 */
extern	int	   num_bands;		/* # of bands per sample	 */

/* ------------------------------------------------------------------------ */

/*
 *  Routines internal to this program.
 */

extern	void	headers(void);
extern	void	interp_image(void);
extern	void	load_table(char	*file);
extern	float	n_interp(int n_dims, DIM_INFO_T *dim_info, float *table,
			 fpixel_t *tuple);
