/*
** NAME
** 	mean -- return mean value of array between start and end
**              start and end given values and counts of data points
**	  	with each value
** 
** SYNOPSIS
**	fpixel_t mean (hist, values, start_pixel, end_pixel)
** 	hist_t *hist;
**	pixel_t start_pixel, end_pixel;
**	fpixel_t *values;
** 
** DESCRIPTION
** 	mean returns the mean pixel value between start_pixel and end_pixel
** 	of the image represented by the given the IPW histogram and
**	corresponding pixel values.
*/

#include "ipw.h"

#include "pgm.h"

fpixel_t
mean (
	hist_t		*hist,			/* IPW histogram	  */
	fpixel_t        *values,		/* FP value of each pixel */
	pixel_t		 start_pixel,		/* start pixel		  */
	pixel_t		 end_pixel)		/* end pixel		  */
{
	fpixel_t	 sum;			/* sum of pixels	  */
	int		 count;			/* count of pixels	  */
	int		 pix;			/* loop counter           */


	if (start_pixel == end_pixel) {
		return (values[start_pixel]);
	} else if (start_pixel > end_pixel) {
		error ("mean: start > end");
	}

   /* sum the pixels between start and end pixels */

	sum = 0.0;
	count = 0;

	for (pix = start_pixel; pix <= end_pixel; pix++) {
		count += hist[pix];
		sum += hist[pix] * values[pix];
	}

	return (sum / count);
}
