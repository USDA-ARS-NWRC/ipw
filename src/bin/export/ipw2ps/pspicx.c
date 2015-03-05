#include "ipw.h"

#include "pixio.h"

#include "pgm.h"

static void
xch(
	REG_1 double   *a,
	REG_2 double   *b)
{
	double          temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * pspicx -- convert image to PostScript
 */

void
pspicx(
	int             fdi,		/* input image file descriptor	 */
	int             nlines,		/* #lines / image		 */
	int             nsamps,		/* #samples / line		 */
	int             nbits,		/* #bits / pixel		 */
	double          h_page,		/* PostScript page height	 */
	double          w_page,		/* PostScript page width	 */
	double          h_img,		/* PostScript image height	 */
	double          w_img,		/* PostScript image width	 */
	bool_t          rotate)		/* ? rotate output image 90 deg	 */
{
 /*
  * hexadecimal numeral map
  */
	static int      _hexdig[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
	};

	double          aspect;
	REG_2 int      *hexdig;		/* fast -> _hexdig		 */
	pixel_t        *ibuf;		/* -> input buffer		 */
	REG_3 pixel_t  *ibufp;		/* -> current input pixel	 */
	REG_5 int       ngot;		/* # pixels in input buffer	 */
	REG_1 int       pixel;		/* current input pixel		 */
	REG_4 int       shift;		/* #bits to shift input pixels	 */

 /*
  * allocate input buffer
  */
 /* NOSTRICT */
	ibuf = (pixel_t *) ecalloc(nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}
 /*
  * set input pixel shift
  */
	shift = (nbits > CHAR_BIT ? nbits - CHAR_BIT : 0);
 /*
  * set image aspect ratio
  */
	if (rotate) {
		xch(&h_img, &w_img);
		xch(&h_page, &w_page);
	}

	aspect = nsamps;
	aspect /= nlines;

	if (aspect >= w_img / h_img) {
		h_img = w_img / aspect;
	}
	else {
		w_img = h_img * aspect;
	}

	assert(h_page >= h_img);
	assert(w_page >= w_img);
 /*
  * write PostScript header
  */
	(void) printf("%%!\n");

	if (rotate) {
		(void) printf("90 rotate\n");
		(void) printf("0 -%d translate\n", ITOP(h_page));
	}

	(void) printf("%d %d translate\n",
		      ITOP(((w_page - w_img) / 2.0) + XTRANS_FUDGE),
		      ITOP((h_page - h_img) / 2.0));
	(void) printf("%d %d scale\n", ITOP(w_img), ITOP(h_img));
	(void) printf("/linebuf %d string def\n", nsamps);
	(void) printf("%d %d %d\n", nsamps, nlines, CHAR_BIT);
	(void) printf("[%d 0 0 -%d 0 %d]\n", nsamps, nlines, nlines);
	(void) printf("{currentfile linebuf readhexstring pop}\n");
	(void) printf("image\n");
 /*
  * write input image pixels as ASCII hexadecimal characters
  */
	hexdig = _hexdig;

	while ((ngot = pvread(fdi, ibuf, nsamps)) > 0) {
		ibufp = ibuf;

		do {
			pixel = *ibufp++;
			pixel >>= shift;

			putchar(hexdig[HI_NYBBLE(pixel)]);
			putchar(hexdig[LO_NYBBLE(pixel)]);
		} while (--ngot > 0);
	}
 /*
  * write PostScript trailer
  */
	(void) printf("showpage\n");
	SAFE_FREE(ibuf);
}
