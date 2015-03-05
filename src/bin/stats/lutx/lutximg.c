#include "ipw.h"

#include "pixio.h"

#include "pgm.h"

/*
 * lutximg--
 */

void
lutximg(
	int             i_fd,		/* input image file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / image line	 */
	REG_6 int       nbands,		/* # image bands		 */
	REG_3 pixel_t **lut,		/* -> LUT array		 	 */
	int             o_fd)		/* output image file descriptor	 */
{
	pixel_t        *buf;		/* -> I/O buffer		 */
	int             line;		/* current line #		 */

 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	buf = (pixel_t *) ecalloc(nsamps * nbands, sizeof(pixel_t));
	if (buf == NULL) {
		error("can't allocate image I/O buffer");
	}
 /*
  * I/O loop
  */
	for (line = 0; line < nlines; ++line) {
		REG_1 pixel_t  *bufp;	/* -> current pixel		 */
		REG_5 int       samp;	/* loop counter			 */

 /*
  * read line
  */
		if (pvread(i_fd, buf, nsamps) != nsamps) {
			error("image read failed, line %d", line);
		}

		bufp = buf;
 /*
  * loop through samples (sample # unimportant)
  */
		samp = nsamps;
		do {
			REG_4 int       band;	/* current band #	 */

 /*
  * loop through bands
  */
			for (band = 0; band < nbands; ++band) {
				REG_2 pixel_t  *lutp;	/* -> lut[pixel] */

 /*
  * map pixel value through lookup table
  */
				lutp = lut[*bufp];
				*bufp++ = lutp[band];
			}
		} while (--samp > 0);
 /*
  * write line
  */
		if (pvwrite(o_fd, buf, nsamps) != nsamps) {
			error("image write failed, line %d", line);
		}
	}

	SAFE_FREE(buf);
}
