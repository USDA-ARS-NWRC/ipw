/*
** NAME
** 	classeq -- classify image by equal area histogram
** 
** SYNOPSIS
**	classeq (fdi, fdm, fdo, nclass, constr, lo, hi, floor, ceil,
**	         infile, maskfile, rep_method)
** 	int fdi, fdm, fdo;
**	int nclass;
**	bool_t constr;
**	fpixel_t lo, hi;
**	pixel_t floor, ceil;
**	char *infile, *maskfile;
**	int rep_method;
** 
** DESCRIPTION
** 	classeq read the given IPW image and mask image files, breaks the
**	data values into 'nclass' different ranges according to equal area
**	histogram and writes the corresponding classified output image.
**	A class range (CR) header is written to the output image specifying
**	the data value ranges for each class identifier.
*/

#include "ipw.h"
#include "fpio.h"
#include "hdrio.h"
#include "gethdrs.h"
#include "bih.h"

#include "pgm.h"

void
classeq (
	int		fdi,		/* input image file descriptor	*/
	int		fdm,		/* mask image file descriptor	*/
	int		fdo,		/* output image file descriptor	*/
	int		nclass,		/* # classes for output image	*/
	bool_t		constr,		/* constrict input to (lo,hi)	*/
	fpixel_t	lo,		/* low data value for input	*/
	fpixel_t	hi,		/* hi data value for input	*/
	pixel_t		floor,		/* output=floor for input<lo	*/
	pixel_t		ceil,		/* output=ceil for input>hi	*/
	char	       *infile,		/* -> filename of input image   */
	char	       *maskfile,	/* -> filename of mask image    */
	char		rep_method)	/* method for rep value         */
{
	int		i;		/* loop counter                 */
	int		nlines;		/* #lines in input image        */
	int		nsamps;		/* #samps in input image        */
	int		sum;		/* sum for cnhist               */
	float	       *cnhist;		/* cumulative normalized hist   */
	float		scale;		/* scale for classification     */
	pixel_t		last_class;	/* class of last pixel          */
	pixel_t	       *lut;		/* lookup table for class       */
	pixel_t		lo_index;	/* index of lo for curr class   */
	pixel_t		start;		/* loop counter                 */
	fpixel_t       *lo_map;		/* low value for each class     */
	fpixel_t       *hi_map;		/* high value for each class    */
	fpixel_t       *rep;		/* rep value for each class     */
	fpixel_t       *values;		/* float values for each pixel  */
	hist_t         *histo;		/* -> histogram			*/
	fpixel_t      **map;		/* floating map			*/
	int	       *maplen;		/* lengths of all maps		*/
	int		nvals;		/* # values in map		*/


	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);

   /* access floating point conversion map */

	map = fpmap (fdi);
	values = map[0];
	maplen = fpmaplen (fdi);
	nvals = maplen[0];

   /* compute histogram for input image */

	histo = hist (fdi, fdm, nvals);

   /* allocate normalized histogram array */

	cnhist = (float *) ecalloc (nvals, sizeof(float));
	if (cnhist == NULL) {
		error("can't allocate cum norm histogram array");
	}


   /* allocate class lookup table */

	lut = (pixel_t *) ecalloc (nvals, sizeof(pixel_t));
	if (lut == NULL) {
		error("can't allocate class lookup table");
	}

   /* allocate lo, hi, and rep map values for classes */

	lo_map = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));
	if (lo_map == NULL) {
		error("can't allocate lo class range array");
	}

	hi_map = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));
	if (hi_map == NULL) {
		error("can't allocate hi class range array");
	}

	rep = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));
	if (rep == NULL) {
		error("can't allocate rep class array");
	}

	if (!constr) {
		lo = values[0];
		hi = values[nvals-1];
	}

   /* get cumulative normalized histogram */

	sum = 0.0;
	for (i = 0; i < nvals; i++) {
		if (values[i] < lo || values[i] >= hi)
			cnhist[i] = 0;
		else {
			sum += histo[i];
			cnhist[i] = sum;
		}
	}

	for (i = 0; i < nvals; i++) {
		cnhist[i] /= sum;
	}

	lo_map[0] = lo;
	hi_map[nclass-1] = hi;
	last_class = 1;

	for (start = 0; values[start] < lo; start++) {
		lut[start] = floor;
	}

	lo_index = start;

   /* create lookup table; get class ranges */

	scale = nclass - 1;

	for (i = start; i < nvals; i++) {

		if (values[i] >= hi)
			break;

		lut[i] = cnhist[i] * scale + 1;
		assert (lut[i] <= nclass);

		if (lut[i] != last_class) {
			hi_map[last_class-1] = lo_map[lut[i]-1] = values[i];
			switch (rep_method) {
			   case MEDIAN:
				rep[last_class-1] =
					median (histo, values, lo_index, i-1);
				break;
			   case MEAN:
				rep[last_class-1] =
					mean (histo, values, lo_index, i-1);
				break;
			   case MIDPOINT:
				rep[last_class-1] = (hi_map[last_class-1] +
					lo_map[last_class-1]) * 0.5;
				break;
			}
			lo_index = i;
			last_class = lut[i];
		}
	}

	switch (rep_method) {
	   case MEDIAN:
		rep[nclass-1] = median (histo, values, lo_index, i);
		break;
	   case MEAN:
		rep[nclass-1] = mean (histo, values, lo_index, i);
		break;
	   case MIDPOINT:
		rep[nclass-1] = (hi_map[nclass-1] + lo_map[nclass-1]) * 0.5;
		break;
	}

	while (i < nvals) {
		lut[i] = ceil;
		i++;
	}

   /* create/write the new CRH header */

	newcrh (fdi, fdo, lo_map, hi_map, rep, floor, ceil, nclass,
		(char *)NULL);

   /* prepare for image output */

	if (boimage (fdo) == ERROR) {
		error ("error terminating header output");
	}

   /* reopen the input image and mask files */

	uclose (fdi);
	fdi = uropen (infile);
	if (fdi == ERROR) {
		error ("can't reopen %s\n", infile);
	}
	skiphdrs (fdi);

	if (fdm != ERROR) {
		uclose (fdm);
		fdm = uropen (maskfile);
		if (fdm == ERROR) {
			error ("can't reopen %s\n", maskfile);
		}
		skiphdrs (fdm);
	}

   /* map the image through the lookup table */

	lutximg (fdi, fdm, nlines, nsamps, lut, fdo);

}
