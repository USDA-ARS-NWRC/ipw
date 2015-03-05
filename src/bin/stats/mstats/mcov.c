#include "ipw.h"

#include "pgm.h"

/*
 * mcov -- print means + variance-covariance matrix
 */

void
mcov(void)
{
	int             class;		/* class #			 */
	int             band;		/* band #			 */

	for (class = 0; class < parm.c_nclasses; ++class) {
		if (parm.c_npixels[class] <= 0) {
			continue;
		}
 /*
  * magic cookie
  */
		(void) printf("#<stats>\n\n");
 /*
  * # bands
  */
		(void) printf("%d\n\n", parm.i_nbands);
 /*
  * mean vector
  */
		for (band = 0; band < parm.i_nbands; ++band) {
			(void) printf("%.*g%s", DBL_DIG,
			     parm.sum_x[class][band] / parm.c_npixels[class],
				   band == parm.i_nbands - 1 ? "\n\n" : " ");
		}
/**
  * variance-covariance matrix.  Element [band1][band2] is:
  *
  *                      sum(band1) * sum(band2)
  * sum(band1 * band2) - -----------------------
  *                                 N
  * ---------------------------------------------
  *                       N
  */
		for (band = 0; band < parm.i_nbands; ++band) {
			int             band2;

			for (band2 = 0; band2 <= band; ++band2) {
				(void) printf("%.*g%s", DBL_DIG,
					   (parm.sum_xy[class][band][band2] -
					    ((parm.sum_x[class][band] *
					      parm.sum_x[class][band2]) /
					     parm.c_npixels[class])) /
					      parm.c_npixels[class],
					      band2 == band ? "\n" : " ");
			}
		}

		(void) printf("\n");

		(void) printf("* %d\n\n", class);

		(void) printf("%d\n\n", parm.c_npixels[class]);
	}
}
