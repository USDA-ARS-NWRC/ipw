/*
 * create new LQH
 */

#include <math.h>

#include "../../horizon.new/hor1d/pgm.h"
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"

LQH_T         **
newlqh(
	int             fdo)		/* output file desc		 */
{
	int             nbands;
	LQH_T         **lqhpp;		/* -> output LQH		 */
	fpixel_t        fbkpt[2][2];	/* limits of LQ			 */
	pixel_t         ibkpt[2][2];	/* integer break points		 */
	char            units[2][32];	/* units			 */
	int             j;

 /*
  * preamble, we use reverse quantization so that dark pixels are those with
  * large horizon angles, i.e. they are shaded
  */
	nbands = hnbands(fdo);
	for (j = 0; j < nbands; ++j) {
		(void) sprintf(units[j], "cos H");

		fbkpt[j][0] = 1;
		fbkpt[j][1] = 0;
		ibkpt[j][0] = 0;
		ibkpt[j][1] = ipow2(hnbits(fdo, j)) - 1;
	}
 /*
  * return the new LQH
  */
 /* NOSTRICT */
	lqhpp = (LQH_T **) hdralloc(nbands, sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	for (j = 0; j < nbands; ++j) {
		lqhpp[j] = lqhmake(hnbits(fdo, j), 2, ibkpt[j], fbkpt[j],
				   units[j], (char *) NULL);
	}

	return (lqhpp);
}
