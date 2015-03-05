/*
** NAME
** 	classify -- classify image by given class information
** 
** SYNOPSIS
**	classify (fdi, fdm, nclass, classes, floor, ceil, fdo);
** 	int fdi, fdm;
**	int nclass;
**	CLASS *classes;
**	pixel_t floor, ceil;
** 	int fdo;
** 
** DESCRIPTION
** 	classify reads the given IPW image file, and writes an image file
**	where each of the image pixels is classified according to the given
**	lo, hi class definition criteria.
*/

#include "ipw.h"
#include "fpio.h"
#include "bih.h"

#include "pgm.h"

void
classify (
	int		fdi,		/* input image file descriptor	*/
	int		fdm,		/* mask image file descriptor	*/
	int		nclass,		/* # classes for output image	*/
	CLASS	       *classes,	/* lo,hi,rep,class# for classes */
	pixel_t		floor,		/* output=floor for input<lo	*/
	pixel_t		ceil,		/* output=ceil for input>hi	*/
	int		fdo)		/* output image file descriptor	*/
{
	int		ilut;		/* lut index                    */
	int		nlines;		/* #lines in input image        */
	int		nsamps;		/* #samps in input image        */
	int		nbits;		/* #bits in input image		*/
	pixel_t	       *lut;		/* lookup table for class       */
	pixel_t		class;		/* current class #		*/
	fpixel_t      **map;		/* -> floating point map	*/
	fpixel_t       *values;		/* -> floating point map	*/
	int	       *maplen;		/* length of maps		*/
	int		nvals;		/* length of FP map		*/


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);
	nbits = hnbits (fdi, 0);


   /* access floating point map */

	map = fpmap (fdi);
	values = map[0];
	maplen = fpmaplen (fdi);
	nvals = maplen[0];

   /* allocate class lookup table */

	lut = (pixel_t *) ecalloc (nvals, sizeof(pixel_t));
	if (lut == NULL) {
		error("can't allocate class lookup table");
	}

   /* initialize lookup table */

	for (ilut = 0; ilut < nvals; ilut++) {

		if (values[ilut] < classes[0].lo) {

			lut[ilut] = floor;

		} else if (values[ilut] >= classes[nclass-1].hi) {

			lut[ilut] = ceil;

		} else {

			lut[ilut] = NO_DATA;
			for (class = 0; class < nclass; class++) {
				if ((values[ilut] >= classes[class].lo) &&
				    (values[ilut] < classes[class].hi)) {
					lut[ilut] = classes[class].cls;
					break;
				}
			}
		}
	}

   /* map the image through the lookup table */

	lutximg (fdi, fdm, nlines, nsamps, lut, fdo);

}
