 /*
  * create and write LQH
  */

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "hdrio.h"
#include "lqh.h"

void
head2(
	int             fdo,		/* output file descriptor	 */
	fpixel_t       *fval)		/* min, max of output LQH	 */
{
	LQH_T         **o_lqh;		/* output LQH			 */
	pixel_t        *ival;		/* min,max pixel values		 */
	pixel_t        *ivalp;		/* min,max pixel values pointer	 */
	fpixel_t       *fvalp;		/* min,max output LQH pointer	 */
	int             nbits = 0;	/* # bits / pixel in output	 */
	int             obands;		/* # output bands		 */
	int             band;		/* band index			 */

	obands = hnbands(fdo);
 /* NOSTRICT */
	ival = (pixel_t *) ecalloc(obands * 2, sizeof(pixel_t));
	assert(ival != NULL);
 /*
  * set pixel_t mapping to 0 and 2^nbits - 1
  */
	for (band = 0; band < obands; ++band) {
		nbits = hnbits(fdo, band);
		assert(nbits > 0);
		ival[2 * band] = 0;
		ival[2 * band + 1] = ipow2(nbits) - 1;
	}
	assert(nbits > 0);
 /*
  * create output LQH
  */
 /* NOSTRICT */
	o_lqh = (LQH_T **) hdralloc(obands, sizeof(LQH_T *), fdo, LQH_HNAME);
	assert(o_lqh != NULL);
	ivalp = ival;
	fvalp = fval;
	for (band = 0; band < obands; ++band) {
		o_lqh[band] = lqhmake(nbits, 2, ivalp, fvalp,
				      (char *) NULL, (char *) NULL);
		if (o_lqh[band] == NULL) {
			error("can't create output LQH");
		}
		ivalp += 2;
		fvalp += 2;
	}
	if (lqhwrite(fdo, o_lqh) == ERROR) {
		error("can't write output LQH");
	}
 /*
  * get ready for rest of output
  */
	if (boimage(fdo) == ERROR) {
		error("can't initialize image output");
	}
	SAFE_FREE(ival);
}
