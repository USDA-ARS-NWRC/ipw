#include <string.h>

#include "ipw.h"
#include "pixio.h"

#include "pgm.h"

/*
 * hdspic -- convert IPW image to HDS sixel format
 */

#define	LINES_PER_SIXEL	6

#define	SIXEL_BIAS	077
#define	SIXEL_NEWLINE	'-'

#define	ENTER_GRAPHICS_MODE	"\033[/w"
#define	EXIT_GRAPHICS_MODE	"\033[2/w"

#define	BEGIN_GRAPHICS_LOAD	"\033Pq"
#define	END_GRAPHICS_LOAD	"\033\\"

void
hdspic(void)
{
	REG_1 pixel_t  *pixel;		/* -> input pixel buffer	 */
	REG_2 uchar_t  *sixel;		/* -> output sixel buffer	 */
	int             nsixels;	/* # sixels / output line	 */
	int             i_line;		/* input line #			 */

 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	pixel = (pixel_t *) ecalloc(parm.i_nsamps, sizeof(pixel_t));
	if (pixel == NULL) {
		error("can't allocate input buffer");
	}

	nsixels = parm.i_nsamps + 1;

 /* NOSTRICT */
	sixel = (uchar_t *) ecalloc(nsixels, sizeof(uchar_t));
	if (sixel == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * set terminal state
  */
	(void) fputs(ENTER_GRAPHICS_MODE, stdout);
	(void) fputs(BEGIN_GRAPHICS_LOAD, stdout);
 /*
  * loop through image lines
  */
	for (i_line = 0; i_line < parm.i_nlines; i_line += LINES_PER_SIXEL) {
		REG_4 int       o_line;	/* line # within sixel		 */

 /*
  * initialize sixel buffer
  */
 /* NOSTRICT */
		(void) memset((char *) sixel, SIXEL_BIAS, (size_t) nsixels);
		sixel[parm.i_nsamps] = SIXEL_NEWLINE;
 /*
  * loop through sixel lines
  */
		for (o_line = 0;
		     o_line < LINES_PER_SIXEL
		     && i_line + o_line < parm.i_nlines;
		     ++o_line) {
			REG_3 int       samp;	/* sample #		 */

 /*
  * read image line
  */
			if (pvread(parm.i_fd, pixel, parm.i_nsamps)
			    != parm.i_nsamps) {
				error("image read error, line %d",
				      i_line + o_line);
			}
 /*
  * merge into sixel buffer
  */
			for (samp = 0; samp < parm.i_nsamps; ++samp) {
				sixel[samp] += pixel[samp] << o_line;
			}
		}
 /*
  * write sixel buffer
  */
 /* NOSTRICT */
		(void) fwrite((char *) sixel, (size_t) nsixels,
			      sizeof(uchar_t), stdout);
 /*
  * this is necessary in case we are talking directly to the tty
  */
		(void) fflush(stdout);
	}
 /*
  * reset terminal state
  */
	(void) fputs(END_GRAPHICS_LOAD, stdout);
	(void) fputs(EXIT_GRAPHICS_MODE, stdout);

	SAFE_FREE(sixel);
	SAFE_FREE(pixel);
}
