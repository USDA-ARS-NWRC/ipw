
#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

/*
 * lqhx -- read old floating-pt values, convert to new, and write
 */

void
lqhx()
{
	fpixel_t       *fpbuf;		/* -> I/O buffer		 */
	int             nsamps;		/* # samples per line		 */
	int             nlines;		/* # lines in image		 */
	int             nbands;		/* # bands per sample		 */

	nbands = hnbands(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
	nlines = hnlines(parm.i_fd);
 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	fpbuf = (fpixel_t *) ecalloc(nsamps * nbands, sizeof(fpixel_t));
	if (fpbuf == NULL) {
		error("can't allocate I/O buffer");
	}
 /*
  * read, convert, write
  */
	while (--nlines >= 0) {
		if (fpvread(parm.i_fd, fpbuf, nsamps) != nsamps) {
			error("read error, line %d",
			      hnlines(parm.i_fd) - nlines + 1);
		}
		if (fpvwrite(parm.o_fd, fpbuf, nsamps) == ERROR) {
			error("write error, line %d",
			      hnlines(parm.o_fd) - nlines + 1);
		}
	}

	SAFE_FREE(fpbuf);
}
