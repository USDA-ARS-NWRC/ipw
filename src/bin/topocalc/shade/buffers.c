 /*
  * allocate buffers returns number of possible values of costbl, etc
  */

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "pgm.h"

void
buffers(
	int             fdi)		/* input file desc		 */
{
	int             nwrite;		/* # pixels to write		 */
	int             nbands;		/* # input bands		 */
	int            *maplen;		/* lengths of fp maps		 */
	fpixel_t      **map;		/* f.p. conversion map		 */

	maplen = fpmaplen(fdi);
	map = fpmap(fdi);
	nwrite = nget = hnsamps(fdi);
	nbands = hnbands(fdi);

 /* NOSTRICT */
	ibuf = (pixel_t *) ecalloc(nbands * nget, sizeof(pixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}

 /* NOSTRICT */
	obuf = (pixel_t *) ecalloc(nwrite, sizeof(pixel_t));
	if (obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * matrix to hold values already computed (dimensions are # poss
  * slopes by # poss azimuths)
  */
 /* NOSTRICT */
	shade = (pixel_t **) allocnd(sizeof(pixel_t), 2, maplen[0], maplen[1]);
	if (shade == NULL) {
		error("can't allocate buffer for computed values");
	}
 /*
  * buffers to hold sines, cosines, and cosines of differences (sines &
  * cosines for each poss slope, cosines of diffs for each poss
  * azimuth)
  */
	sintbl = map[0];
 /* NOSTRICT */
	costbl = (float *) ecalloc(maplen[0], sizeof(float));
 /* NOSTRICT */
	cosdtbl = (float *) ecalloc(maplen[1], sizeof(float));
	if (sintbl == NULL || costbl == NULL || cosdtbl == NULL) {
		error("can't allocate buffers for trig tables");
	}
}
