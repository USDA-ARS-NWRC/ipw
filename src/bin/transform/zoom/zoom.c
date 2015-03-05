#include "ipw.h"
#include "pixio.h"

#include "pgm.h"

/*
 * zoom -- image zooming ({mag,min}ify by {replication,subsampling})
 */

void
zoom(void)
{
	int             i_line;		/* input image line counter	 */
	int             o_line;		/* output image line counter	 */

 /*
  * allocate buffer; misc. initializations
  */
 /* NOSTRICT */
	parm.buf = (pixel_t *) ecalloc(MAX(parm.i_nsamps, parm.o_nsamps),
				       parm.nbands * sizeof(pixel_t));
	if (parm.buf == NULL) {
		error("can't allocate image line buffer");
	}

	o_line = 0;
 /*
  * loop through input lines
  */
	for (i_line = 0; i_line < parm.i_nlines; ++i_line) {
		int             n;	/* loop counter			 */

		if (pvread(parm.i_fd, parm.buf, parm.i_nsamps)
		    != parm.i_nsamps) {
			error("image read error, line %d", i_line);
		}
 /*
  * if minifying lines then possibly skip this line
  */
		if (i_line % parm.skip_lines != 0) {
			continue;
		}
 /*
  * skip samples
  */
		if (parm.skip_samps > 1) {
			subsamp();
		}
 /*
  * replicate samples
  */
		else if (parm.dup_samps > 1) {
			replicate();
		}
 /*
  * write line (if magnifying lines then repeat)
  */
		n = parm.dup_lines;
		do {
			if (pvwrite(parm.o_fd, parm.buf, parm.o_nsamps)
			    != parm.o_nsamps) {
				error("image write error, line %d", o_line);
			}

			++o_line;
		} while (--n > 0);
	}
}
