/*
 * trigonometric look-up table
 */

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

void
trigtbl(void)
{
	fpixel_t      **smap;		/* slope/azm map		 */
	int             band;
	int             horbnd;		/* # horizon bands		 */
	int             j;
	int            *hmaplen;	/* length of horizon tbl	 */
	int            *smaplen;	/* length of slope tbl		 */

	smaplen = fpmaplen(parm.i_fds);
	hmaplen = fpmaplen(parm.i_fdh);
	horbnd = hnbands(parm.i_fdh);
	smap = fpmap(parm.i_fds);

 /*
  * cosines of slopes
  */
	for (j = smaplen[0] - 1; j >= 0; --j) {
		parm.cstbl[j] = sqrt((1 - parm.sstbl[j]) * (1 + parm.sstbl[j]));
	}
 /*
  * sines and values of horizon angles from zenith
  */
	for (band = 0; band < horbnd; ++band) {
		for (j = hmaplen[band] - 1; j >= 0; --j) {
			parm.sh2tbl[band][j] = (1 - parm.chtbl[band][j]) *
				(1 + parm.chtbl[band][j]);
			parm.hdtbl[band][j] = acos(parm.chtbl[band][j]) -
				sqrt(parm.sh2tbl[band][j]) *
				parm.chtbl[band][j];
		}
	}
 /*
  * cosines of differences between horizon azimuth & slope aspect
  */
	for (band = 0; band < horbnd; ++band) {
		for (j = smaplen[1] - 1; j >= 0; --j) {
			parm.cosdtbl[band][j] = cos(parm.hazm[band] -
						    smap[1][j]);
		}
	}
}
