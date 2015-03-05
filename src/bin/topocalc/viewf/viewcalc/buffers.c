/*
 * allocate buffers
 */

#include "ipw.h"
#include "bih.h"
#include "fpio.h"
#include "pgm.h"

void
buffers(void)
{
	int             npix;		/* # pixels to read/write	 */
	int             horbnd;		/* # horizon bands		 */
	int            *smaplen;	/* length of slope map		 */
	int            *hmaplen;	/* length of horizon map	 */
	fpixel_t      **map;		/* slope map			 */
	fpixel_t      **hmap;		/* horizon map			 */
	int             j;

	smaplen = fpmaplen(parm.i_fds);
	hmaplen = fpmaplen(parm.i_fdh);
	map = fpmap(parm.i_fds);
	hmap = fpmap(parm.i_fdh);

	npix = hnsamps(parm.i_fds);
	if (npix != hnsamps(parm.i_fdh)) {
		error("# samples different in two input files");
	}
	if (hnbands(parm.i_fds) != 2) {
		error("input slope/azm file must have 2 bands");
	}
	horbnd = hnbands(parm.i_fdh);

 /* NOSTRICT */
	parm.sbuf = (pixel_t *) ecalloc(2 * npix, sizeof(pixel_t));
	if (parm.sbuf == NULL) {
		error("can't allocate slope/azm buffer");
	}
 /* NOSTRICT */
	parm.hbuf = (pixel_t *) ecalloc(horbnd * npix, sizeof(pixel_t));
	if (parm.hbuf == NULL) {
		error("can't allocate horizon buffer");
	}

 /* NOSTRICT */
	parm.obuf = (fpixel_t *) ecalloc(npix * 2, sizeof(fpixel_t));
	if (parm.obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * buffers for trig values
  */

	parm.sstbl = map[0];
 /* NOSTRICT */
	parm.cstbl = (float *) ecalloc(smaplen[0], sizeof(float));
	parm.chtbl = hmap;
 /* NOSTRICT */
	parm.cosdtbl = (float **) allocnd(sizeof(float), 2, horbnd, smaplen[1]);
 /* NOSTRICT */
	parm.sh2tbl = (float **) ecalloc(horbnd, sizeof(float *));
 /* NOSTRICT */
	parm.hdtbl = (float **) ecalloc(horbnd, sizeof(float *));
	if (parm.sh2tbl == NULL || parm.cstbl == NULL ||
	    parm.chtbl == NULL || parm.cosdtbl == NULL) {
		error("can't allocate buffers for trig tables");
	}
	for (j = 0; j < horbnd; ++j) {
 /* NOSTRICT */
		parm.sh2tbl[j] = (float *) ecalloc(hmaplen[j], sizeof(float));
 /* NOSTRICT */
		parm.hdtbl[j] = (float *) ecalloc(hmaplen[j], sizeof(float));
		if (parm.sh2tbl[j] == NULL || parm.hdtbl[j] == NULL) {
			error("can't allocate buffers for trig tables");
		}
	}
}
