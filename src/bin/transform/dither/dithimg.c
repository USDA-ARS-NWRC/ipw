#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "pgm.h"

/*
 * dithimg -- dither an image down to 1 bit / pixel
 */

#define	IN_NLEVELS	256

void
dithimg(
	int             i_fd,		/* input image file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / image line	 */
	int             nbands,		/* # image bands		 */
	pixel_t      ***dm,		/* -> dither matrix		 */
	int             rank,		/* dither matrix rank		 */
	int             o_fd)		/* output image file descriptor	 */
{
	pixel_t        *buf;		/* -> I/O buffer		 */
	REG_6 int       idx_mask;	/* dither matrix index mask	 */
	int             line;		/* current line #		 */

 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	buf = (pixel_t *) ecalloc(nsamps * nbands, sizeof(pixel_t));
	if (buf == NULL) {
		error("can't allocate I/O buffer");
	}
 /*
  * construct dither matrix index mask
  * 
  * NB: this assumes rank is a power of 2
  */
	idx_mask = rank - 1;
 /*
  * process a line at a time
  */
	for (line = 0; line < nlines; ++line) {
		REG_1 pixel_t  *bufp;	/* -> current pixel in buf	 */
		REG_3 pixel_t **dm_line;/* -> dither matrix line	 */
		REG_5 int       samp;	/* current sample #		 */

		if (pvread(i_fd, buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}

		bufp = buf;
		dm_line = dm[line & idx_mask];

		for (samp = nsamps; --samp >= 0;) {
			REG_4 int       band;	/* current band #	 */
			REG_2 pixel_t  *dm_samp;	/* -> d.m. samp	 */

			dm_samp = dm_line[samp & idx_mask];

			band = nbands;
			do {
				if (*bufp < *dm_samp++) {
					*bufp++ = BLACK;
				}
				else {
					*bufp++ = WHITE;
				}
			} while (--band > 0);
		}

		if (pvwrite(o_fd, buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}
 /*
  * make sure there's nothing left to be read
  */
	if (!ueof(i_fd)) {
		error("input image is larger than header indicates");
	}

	SAFE_FREE(buf);
}
