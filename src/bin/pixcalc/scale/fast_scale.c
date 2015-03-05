#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

/*
 * fast_scale -- scale an image by scaling the floating point map (LQ) header
 */

void
fast_scale (
	char		aop,		/* operation		 	 */
	float		val)		/* value		 	 */
{
	int             nlines;		/* # lines in image		 */
	int             nsamps;		/* # pixels per line		 */
	int             line;		/* line index			 */
	pixel_t        *buf;		/* -> I/O buffer		 */
	fpixel_t       *fpmin;		/* F.P. min for each band	 */
	fpixel_t       *fpmax;		/* F.P. min for each band	 */


   /* get floating point min and max from input image LQ header */

	fpmin = fpfmin (parm.i_fd);
	fpmax = fpfmax (parm.i_fd);

   /* calculate new scaled breakpoints for LQ header */
	
	switch (aop) {
		case OP1:
			parm.mmval[0] = fpmin[0] + val;
			parm.mmval[1] = fpmax[0] + val;
			break;
		case OP2:
			parm.mmval[0] = fpmin[0] - val;
			parm.mmval[1] = fpmax[0] - val;
			break;
		case OP3:
			parm.mmval[0] = fpmin[0] * val;
			parm.mmval[1] = fpmax[0] * val;
			break;
		case OP4:
			if (val == 0.0) {
				error("scaling value cannot be 0.0 for divide operation");
			}
			parm.mmval[0] = fpmin[0] / val;
			parm.mmval[1] = fpmax[0] / val;
			break;
		case OP5:
			parm.mmval[0] = pow ((double)fpmin[0], val);
			parm.mmval[1] = pow ((double)fpmax[0], val);
			break;
	}

   /* write new LQ header */

	headers (TRUE);

	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);

   /* allocate I/O line buffer */

	buf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
	if (buf == NULL) {
		error ("can't allocate I/O buffer");
	}

   /* copy input image lines to output image (raw format) */

	for (line = 0; line < nlines; line++) {

		if (pvread (parm.i_fd, buf, nsamps) != nsamps) {
			error ("Error reading input image, line %d", line);
		}

		if (pvwrite (parm.o_fd, buf, nsamps) != nsamps) {
			error ("Error writing output image, line %d", line);
		}
	}
}
