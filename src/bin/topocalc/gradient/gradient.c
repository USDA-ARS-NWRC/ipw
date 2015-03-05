/*
** orchestrate computation of gradient
**
**	In trying to understand this, remember that the definition of
**	x- and y-coordinates is the standard cartographic one:
**	y increases from west to east and x increases from north to south.
**
**	read input data, allocate buffers,
**	call routine to compute gradient for each pixel
*/

#include "ipw.h"
#include "fpio.h"
#include "bih.h"

#include "pgm.h"

void
gradient(
	int             fdi,		/* input file descriptor	 */
	int             fdo,		/* output file descriptor	 */
	bool_t          slope,		/* ? slope only			 */
	bool_t          aspect,		/* ? aspect only		 */
	fpixel_t       *spacing)	/* grid spacing (1-line 2-samp)	 */
{
	fpixel_t       *a;		/* vector of aspect (rads)	 */
	fpixel_t       *dx;		/* finite diffs in x-direction	 */
	fpixel_t       *dy;		/* finite diffs in y-direction	 */
	fpixel_t      **ibuf;		/* input buffers		 */
	fpixel_t       *obuf;		/* output buffer		 */
	fpixel_t       *s;		/* vector of sinS		 */
	fpixel_t       *temp;		/* for exchange			 */
	int             j;		/* counter			 */
	int             ngot;		/* # samples read		 */
	int             nsamps;		/* # samples / line		 */

 /*
  * Allocate 3 input buffers of length nsamps+2. We need the extra 2
  * spaces on end to calculate finite differences at end
  */
	nsamps = hnsamps(fdi);
 /* NOSTRICT */
	ibuf = (fpixel_t **) allocnd(sizeof(fpixel_t), 2, 3, nsamps + 2);
	if (ibuf == NULL) {
		error("Can't allocate input buffer");
	}
 /*
  * allocate buffers to hold dx, dy
  */
 /* NOSTRICT */
	dx = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
 /* NOSTRICT */
	dy = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	if (dx == NULL || dy == NULL) {
		error("Can't allocate dx & dy vectors");
	}
 /*
  * allocate buffers to hold s, a
  */

	s = NULL;
	a = NULL;

	if (slope) {
 /* NOSTRICT */
		s = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	}
	if (aspect) {
 /* NOSTRICT */
		a = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	}
	if ((slope && s == NULL) || (aspect && a == NULL)) {
		error("Can't allocate s and/or a vectors");
	}

 /*
  * allocate output buffer (we only need a special buffer if we're
  * going to merge slope and aspect later)
  */
	if (slope && aspect) {
 /* NOSTRICT */
		if ((obuf = (fpixel_t *)
		     ecalloc(2 * nsamps, sizeof(fpixel_t))) == NULL) {
			error("Can't allocate output buffer");
		}
	}

	else if (slope) {
		obuf = s;
	}

	else {
		obuf = a;
	}

 /*
  * Because we allow an extra pixel at beginning and end of line,
  * increment input buffers to 'first' pixel in calculation
  */

	for (j = 0; j < 3; ++j) {
		++(ibuf[j]);
	}

 /*
  * initialization: read 1st line into buffer[1], 2nd line into
  * buffer[2]
  */

	for (j = 1; j <= 2; ++j) {
		if (fpvread(fdi, ibuf[j], nsamps) != nsamps)
			error("Read error, line %d", j);
	}
 /*
  * fill end pixels of line 1 and all of line 0
  */
	fillstart(nsamps, ibuf[0], ibuf[1], ibuf[2]);
 /*
  * loop: calculate and write gradients then shift buffers and read
  * next line
  */
	do {
 /*
  * compute
  */
		gradu(nsamps, spacing, slope, aspect, ibuf, dx, dy, s, a, obuf);
 /*
  * write output line
  */
		if (fpvwrite(fdo, obuf, nsamps) != nsamps) {
			error("Can't write line");
		}

 /*
  * switch buffers to prepare for next read
  */

		temp = ibuf[0];
		ibuf[0] = ibuf[1];
		ibuf[1] = ibuf[2];
		ibuf[2] = temp;

 /*
  * read next line
  */

	} while ((ngot = fpvread(fdi, ibuf[2], nsamps)) > 0);

	if (ngot < 0) {
		error("Image read error");
	}

 /*
  * still have to do the last line
  */
	fillstart(nsamps, ibuf[2], ibuf[0], ibuf[1]);
	gradu(nsamps, spacing, slope, aspect, ibuf, dx, dy, s, a, obuf);
 /*
  * write last line
  */
	if (fpvwrite(fdo, obuf, nsamps) != nsamps) {
		error("Can't write line");
	}

 /*
  * clean up
  */
	SAFE_FREE(ibuf);
	SAFE_FREE(dx);
	SAFE_FREE(dy);

	if (slope) {
		SAFE_FREE(s);
	}
	if (aspect) {
		SAFE_FREE(a);
	}
	if (slope && aspect) {
		SAFE_FREE(obuf);
	}
}
