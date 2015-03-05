/*
** NAME
** 	cnhist -- compute and output cumulative normalized histograms
** 
** SYNOPSIS
**	#include "getargs.h"
**
**	cnhist (nfiles, operands, fdm)
**	int nfiles;
**	OPTION_T operands;
**	int fdm;
** 
** DESCRIPTION
** 	cnhist read each of the given input images, calculates a cumulative
**	normalized histogram of all of the data and writes the results to
**	standard output.
** 
*/

#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
cnhist (
	int		nfiles,		/* # input files		 */
	OPTION_T	operands,	/* option struct with filenames	 */
	int		fdm)		/* mask file descriptor		 */
{
	int		fdi =ERROR;	/* input image file descriptor	 */
	int		ifile;		/* file loop counter		 */
	int		nlines;		/* # lines in input image(s)	 */
	int		nsamps;		/* # samps in input image(s)	 */
	int		nbits;		/* # bits in input image(s)	 */
	int		sum;		/* sum of pixel counts		 */
	int		pixcount;	/* total count of unmasked pixels*/
	int		i;		/* loop counter			 */
	int		npixels;	/* total # pixels in mask image	 */
	int		nhist;		/* length of histogram		 */
	int	       *hist;		/* histogram array		 */
	int	       *fmaplen;	/* -> lengths of all F.P. maps	 */
	int		maplen;		/* length of band 0 F.P. map	 */
	fpixel_t      **fmap;		/* -> all F.P. maps		 */
	fpixel_t       *map;		/* F.P. map for band 0		 */
	pixel_t	       *ibuf;		/* input image line buffer	 */
	pixel_t	       *mbuf =NULL;	/* entire mask image		 */



   /* check headers of all images */

	check_headers (nfiles, operands, fdm, &nbits, &nlines, &nsamps);

   /* allocate histogram array */

	nhist = ipow2 (nbits);
	hist = (int *) ecalloc (nhist, sizeof(int));
	if (hist == NULL) {
		error ("Unable to allocate histogram array");
	}

   /* allocate input image line buffer */

	ibuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("Unable to allocate input line buffer");
	}

   /* if mask provided, read entire image */

	if (fdm != ERROR) {
		npixels = nsamps * nlines;
		mbuf = (pixel_t *) ecalloc (npixels, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("Unable to allocate mask buffer");
		}
		if (pvread (fdm, mbuf, npixels) != npixels) {
			error ("Error reading mask image");
		}
	}


   /* loop reading images and accumulating histograms */

	ifile = 0;
	pixcount = 0;

	do {
		/* close last input image */

		if (ifile > 0)
			uclose (fdi);

		/* open next input image */

		if (ifile == 0 && nfiles == 0) {
			fdi = ustdin();
		} else {
			fdi = uropen (str_arg (operands, ifile));
			if (fdi == ERROR) {
				error ("Unable to open file %s",
					str_arg (operands, ifile));
			}
		}
		no_tty (fdi);

		/* read headers */

		read_headers (fdi);

		/* accumulate histogram */

		accum (fdi, fdm, ibuf, mbuf, nlines, nsamps, hist, &pixcount);

		ifile++;

	} while (ifile < nfiles);

   /* access floating point map of last input image */

	fmaplen = fpmaplen (fdi);
	maplen = fmaplen[0];
	fmap = fpmap (fdi);
	map = fmap[0];

   /* convert histogram table to cumulative counts */

	sum = 0;
	for (i = 0; i < nhist; i++) {
		sum += hist[i];
		hist[i] = sum;
	}

   /* print CN histogram */

	for (i = 0; i < nhist; i++) {
		printf ("%.2f\t%6.1f\n", map[i], (hist[i] * 100.0) / sum);
	}
}
