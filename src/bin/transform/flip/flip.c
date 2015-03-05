/*
** flip - read it; flip lines, samps or both; and write
*/

#include "ipw.h"
#include "bih.h"
#include "pgm.h"

void
flip(void)
{
	addr_t          buf;		/* -> 1-d or 2-d data buffer	 */
	int             sampbytes;	/* # bytes per sample		 */
	int             linebytes;	/* # bytes per line		 */
	int             nlines;		/* # lines			 */
	int             nsamps;		/* # samps			 */
	long            hold;

 /*
  * If lines are to be flipped, allocate a 2-d image buffer (this could
  * be large, and cause the program to fail here). Otherwise, allocate
  * an image buffer large enough to hold one line.
  */

	nsamps = hnsamps(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	sampbytes = sampsize(parm.i_fd);
	hold = sampbytes;
	hold *= nsamps;
	linebytes = ltoi(hold);

	if (parm.lines) {
 /* NOSTRICT */
		buf = allocnd(sampbytes, 2, nlines, nsamps);
		if (buf == (addr_t) NULL) {
			error("can't allocate 2-d array");
		}
	}

	else if (parm.samps) {
		buf = (addr_t) malloc((unsigned) linebytes);
		if (buf == (addr_t) NULL) {
			error("can't allocate 1-d vector");
		}
	}

	else {
 /* shouldn't reach, as checked in main also */
		bug("neither lines nor samps switch on");
	}

	if (parm.lines) {

 /* NOSTRICT */
		flip2d(nlines, nsamps, sampbytes, (addr_t *) buf);
	}

	else if (parm.samps) {
		flip1d(nlines, nsamps, sampbytes, buf);
	}

	SAFE_FREE(buf);
}
