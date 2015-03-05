#include "ipw.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 * mstats -- image multivariate statistics
 */

void
mstats(void)
{
	int             line;		/* image line #			 */

	init();

	for (line = 0; line < parm.nlines; ++line) {
		if (fpvread(parm.i_fd, parm.i_buf, parm.nsamps)
		    != parm.nsamps) {
			error("image read error, line %d", line);
		}

		if (parm.c_fd != ERROR) {
			if (pvread(parm.c_fd, parm.c_buf, parm.nsamps)
			    != parm.nsamps) {
				error("image read error, line %d", line);
			}
		}

		accum();
	}

	mcov();
}
