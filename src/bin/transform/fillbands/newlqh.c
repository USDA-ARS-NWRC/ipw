/*
 * create new LQH
 */
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"

LQH_T **newlqh(
	int             fdo,		/* output file desc		 */
	LQH_T         **i_lqh)		/* -> input LQH		 	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	fpixel_t        fbkpt[2];	/* limits of LQ			 */
	pixel_t         ibkpt[2];	/* integer break points		 */
	char           *units;		/* units			 */
	int             j;
	fpixel_t       *band0map;	/* band 0 lq limits 		 */
	fpixel_t       *band1map;	/* band 1 lq limits 		 */
	int             maplen;		/* length of lq map arrays       */


	band0map = lqh_map(i_lqh[0]);
	band1map = lqh_map(i_lqh[1]);
	maplen = lqh_maplen(i_lqh[0]) - 1;	
	units = lqh_units(i_lqh[0]);

/* get extrema of input breakpoints */

	fbkpt[0] = MIN(band0map[0], band1map[0]);
	fbkpt[1] = MAX(band0map[maplen], band1map[maplen]);

 /*
  * return the new LQH
  */
	ibkpt[0] = 0;
	ibkpt[1] = ipow2(hnbits(fdo, 0)) - 1;

	lqhpp = (LQH_T **) hdralloc(hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	for (j = hnbands(fdo); --j >= 0;)
		lqhpp[j] = lqhmake(hnbits(fdo, j), 2,
				ibkpt, fbkpt, units, (char *) NULL);

	return (lqhpp);
}

