#include "ipw.h"

#include "bih.h"
#include "pixio.h"

#include "pgm.h"

/*
 * mk_dm -- make dither matrix
 */

pixel_t      ***
mk_dm(
	int             rank,		/* dither matrix rank		 */
	int             nbands,		/* # image bands		 */
	BIH_T         **bihpp)		/* -> basic image header	 */
{
	pixel_t      ***dm;		/* -> dither matrix		 */
	int             line;		/* current line #		 */

 /*
  * allocate dither matrix
  */
 /* NOSTRICT */
	dm = (pixel_t ***) allocnd(sizeof(pixel_t), 3, rank, rank, nbands);
	if (dm == NULL) {
		error("can't allocate %dx%dx%d dither matrix",
		      rank, rank, nbands);
	}
 /*
  * initialize dither matrix: each band is scaled according to #bits / pixel
  * in that band
  */
	for (line = 0; line < rank; ++line) {
		int             samp;	/* current sample #		 */

		for (samp = 0; samp < rank; ++samp) {
			int             band;	/* current band #	 */

			for (band = 0; band < nbands; ++band) {
				dm[line][samp][band] =
					dm_val(rank, line, samp) *
					ipow2(bih_nbits(bihpp[band])) /
					(rank * rank);
			}
		}
	}

	return (dm);
}
