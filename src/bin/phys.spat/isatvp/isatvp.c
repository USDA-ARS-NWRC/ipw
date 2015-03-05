/*
** NAME
**      isatvp - convert dew point temp to saturation vapor press
**
** SYNOPSIS
**	void isatvp (fdi, fdm, fdo)
**	int fdi;
**	int fdm;
**	int fdo;
** 
** DESCRIPTION
**      isatvp converts dew point temp (C) to sat vapor press (Pa)
** 
** HISTORY
**	10/21/92: Written by D. Marks, USGS, EPA, ERL/C;
**
*/

#include        <math.h>
#include        "ipw.h"
#include	"pgm.h"
#include        "bih.h"
#include        "pixio.h"
#include        "fpio.h"
#include        "envphys.h"

void
isatvp (
	int	fdi,		/*	input image file descriptor	*/
	int	fdm,		/*	mask image file descriptor	*/
	int	fdo)		/*	output image file descriptor	*/
{
	float	dpt;		/*	dew point temp			*/
        float	svp;		/*	saturation vapor pressure	*/
        float	svp_min =0.0;	/*	min svp in output image		*/
        float	svp_max =0.0;	/*	max svp in output image		*/

	fpixel_t       *ibuf;		/* pointer to input buffer	 */
	fpixel_t       *ibufp;		/* pointer in input buffer	 */
	fpixel_t       *obuf;		/* pointer to output buffer	 */
	fpixel_t       *obufp;		/* pointer in output buffer	 */
	pixel_t	       *mbuf;		/* pointer to mask buffer	 */
	pixel_t	       *mbufp;		/* pointer in mask buffer	 */
	int		nsamps;		/* # samples per line		 */
	int		nlines;		/* # lines in image		 */
	int		samp;		/* sample loop counter		 */
	int		line;		/* line loop counter		 */
	int		first_pixel;	/* flag for first pixel proc`ed  */
	int		fdt;		/* temp output file desc	 */
	int		nbytes;		/* #bytes to write		 */

	char	       *tempfile;	/* temporary output file	 */

	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;


   /* open temporary output file */

	tempfile = mktemplate("isatvp");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* Allocate input buffer */

	ibuf = (fpixel_t *) ecalloc (nsamps * IBANDS, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate input buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

	first_pixel = TRUE;

	
   /* read input data and do calculations */

	for (line=0; line < nlines; line++) {

		/* read line of image */

		if (fpvread (fdi, ibuf, nsamps) != nsamps) {
			error ("read error, line %d", line);
		}

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* Do calculations on each sample of line */

		ibufp = ibuf;
		mbufp = mbuf;
		obufp = obuf;

		for (samp=0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp++) {

				/* extract input band */

				dpt = *ibufp++;

				/*	convert dew point temp to K	*/

				dpt += FREEZE;

				if (dpt < 0.0)
					error("unrealistic dew point temperature: %g", dpt);
	
				/*	calculate saturation vapor pressure	*/

				svp = (float) sati((double) dpt);

				/* set output band */

				*obufp++ = svp;

				/* update min/max */

				if (!first_pixel) {
					svp_min = MIN (svp_min, svp);
					svp_max = MAX (svp_max, svp);
				} else {
					first_pixel = FALSE;
					svp_min = svp;
					svp_max = svp;
				}

			} else {
				*obufp++ = 0.0;
				ibufp += IBANDS;
			}
		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

   /* create/write LQH for output image */

	newlqh (fdo, svp_min, svp_max);

   /* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);

}
