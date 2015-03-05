#include "ipw.h"
#include "image.h"

IMAGE_T *
open_mask(
	char    * file,		/* filename of mask image	*/
	int	  inLines,	/* # of lines in input image	*/
	int	  inSamples)	/* # of samples in input image	*/
{
	IMAGE_T * mask;		/* -> mask image		*/

	assert(file != NULL);
	assert(inLines > 0);
	assert(inSamples > 0);

	mask = open_image(file, PIXEL_ACCESS);
	if (mask == NULL) {
		error("can't open mask image \"%s\"", file);
	}

	/*
	 *  Check the # of lines & samples
	 */
	if (nlines(mask) != inLines) {
		error("# of lines in input image (%d) and mask (%d) differ",
		      inLines, nlines(mask));
	}
	if (nsamples(mask) != inSamples) {
		error("# of samples in input image (%d) and mask (%d) differ",
		      inSamples, nsamples(mask));
	}

	/*
	 *  Must be 1 band image
	 */
	if (nbands(mask) != 1) {
		error("mask is not a single-band image");
	}

	return mask;
}
