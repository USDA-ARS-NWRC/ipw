/*
** NAME
** 	median -- return median value of array between start and end
**                start and end given values and counts of data points
**		  with each value
** 
** SYNOPSIS
**	fpixel_t median (hist, values, start_pixel, end_pixel)
** 	hist_t *hist;
**	pixel_t start_pixel, end_pixel;
**	fpixel_t *values;
** 
** DESCRIPTION
** 	median returns the median pixel value between start_pixel and end_pixel
** 	of the image represented by the given the IPW histogram and
**	corresponding pixel values.
*/

#include "ipw.h"

#include "pgm.h"

#define EVEN(n)		(n & 1 == 0)

fpixel_t
median (
	hist_t		*hist,			/* IPW histogram	  */
	fpixel_t        *values,		/* FP value of each pixel */
	pixel_t		 start_pixel,		/* start pixel		  */
	pixel_t		 end_pixel)		/* end pixel		  */
{
	int		 count;			/* count of pixels	  */
	int		 mindex;		/* index of median        */
	int		 pix;			/* loop counter           */
	int		 even;			/* TRUE if count is even  */


	if (start_pixel == end_pixel) {
		return (values[start_pixel]);
	} else if (start_pixel > end_pixel) {
		error ("median: start > end");
	}

   /* count the pixels between start and end pixels */

	count = 0;

	for (pix = start_pixel; pix <= end_pixel; pix++)
		count += hist[pix];

	even = EVEN (count);

	mindex = (count + 1) / 2;

   /* find median pixel */

	count = 0;

	for (pix = start_pixel; pix <= end_pixel; pix++) {
		count += hist[pix];
		if (count > mindex) {
			return (values[pix]);
		} else if (count == mindex) {
			if (even) {
				return ((values[pix+1] + values[pix]) / 2);
			} else {
				return (values[pix]);
			}
		}
	}

	error ("reached end of median calculation without an answer");
}
