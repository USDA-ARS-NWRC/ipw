#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"

#include "pgm.h"

/*
 * wedge -- orchestrate linear combination of line, sample coordinates
 */

void
wedge(
	int             fdi,		/* input image file descriptor	 */
	int             nbits,		/* # bits / pixel in output	 */
	aint_t          ncoef,		/* # coeffs on command line	 */
	areal_t        *cval,		/* coeffs from command line	 */
	int             fdo)		/* output image file descriptor	 */
{
	FREG_1 fpixel_t fpix;		/* sample number		 */
	FREG_2 fpixel_t fline;		/* line index			 */
	REG_1 fpixel_t *cfpval;		/* same as cval			 */
	REG_2 fpixel_t *op;		/* -> obuf			 */
	REG_3 int       ob;		/* current output band #	 */
	REG_4 int       obands;		/* # output bands		 */
	REG_5 int       pixel;		/* sample number		 */
	fpixel_t       *fval;		/* min,max output values	 */
	fpixel_t        temp;		/* temporary storage		 */
	fpixel_t       *obuf;		/* output buffer		 */
	int             j;		/* coefficient index		 */
	int             line;		/* line index			 */
	int             nlines;		/* # lines in image		 */
	int             nsamps;		/* # samples / line		 */

 /*
  * read all headers, write output BIH
  */
	head1(fdi, nbits, ncoef, fdo);
	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);
	obands = hnbands(fdo);
 /*
  * allocate output buffer
  */
 /* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(obands * nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * copy coefficients into array of fpixel_t type
  */
 /* NOSTRICT */
	cfpval = (fpixel_t *) ecalloc(ncoef, sizeof(fpixel_t));
	assert(cfpval != NULL);
	for (j = 0; j < ncoef; ++j) {
		cfpval[j] = cval[j];
	}
 /*
  * find min and max of output vector
  */
 /* NOSTRICT */
	fval = (fpixel_t *) ecalloc(obands * 2, sizeof(fpixel_t));
	assert(fval != NULL);
	for (ob = 0; ob < obands; ++ob) {
		if (cfpval[2 * ob] == 0 && cfpval[2 * ob + 1] == 0) {
			error("band %d: both coefficients cannot be 0", ob);
		}
		fval[2 * ob] = 0;
		fval[2 * ob + 1] = cfpval[2 * ob] * (nlines - 1) +
			cfpval[2 * ob + 1] * (nsamps - 1);
		if (fval[2 * ob] > fval[2 * ob + 1]) {
			temp = fval[2 * ob];
			fval[2 * ob] = fval[2 * ob + 1];
			fval[2 * ob + 1] = temp;
		}
	}
 /*
  * create and write LQH
  */
	head2(fdo, fval);
	SAFE_FREE(fval);
 /*
  * cycle thru line, sample coords and write output image
  */
	for (fline = 0, line = nlines; --line >= 0; ++fline) {
		op = obuf;
		for (fpix = 0, pixel = nsamps; --pixel >= 0; ++fpix) {
			for (ob = 0; ob < obands; ++ob) {
				*op++ = cfpval[2 * ob] * fline +
					cfpval[2 * ob + 1] * fpix;
			}
		}
 /*
  * write the line
  */
		if (fpvwrite(fdo, obuf, nsamps) != nsamps) {
			error("output error");
		}
	}
 /*
  * all done
  */
	SAFE_FREE(obuf);
	SAFE_FREE(cfpval);
}
