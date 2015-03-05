/*
** NAME
** 	newlqh -- create new LQH header for gsunlight program
** 
** SYNOPSIS
**	newlqh (fdo)
** 
** DESCRIPTION
** 	newlqh creates a two-band linear quantization header for the
**	gsunlight output image.
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"
#include "pgm.h"

LQH_T         **
newlqh (
	int             fdo)			/* output file desc	 */
{
	LQH_T         **lqhpp;			/* -> output LQH	 */
	fpixel_t        fbkpt[OBANDS][2];	/* limits of LQ		 */
	pixel_t         ibkpt[OBANDS][2];	/* integer break points	 */
	char           *units[OBANDS];		/* units		 */
	int             j;

   /* band 0: cos zenith angle (between 0 and 1) */

	fbkpt[0][0] = 0;
	fbkpt[0][1] = 1;
	units[0] = "cosZ";

   /* band 1: azimuth in degrees (between -180 and 180) */

	fbkpt[1][0] = -180;
	fbkpt[1][1] = 180;
	units[1] = "degrees";

   /* create the new LQH */

	ibkpt[0][0] = ibkpt[1][0] = 0;
	ibkpt[0][1] = ipow2(hnbits(fdo, 0)) - 1;
	ibkpt[1][1] = ipow2(hnbits(fdo, 1)) - 1;

	lqhpp = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *), fdo, LQH_HNAME);

	for (j = 0; j < OBANDS; j++) {
		lqhpp[j] = lqhmake (hnbits(fdo, j), 2,
				ibkpt[j], fbkpt[j], units[j], (char *) NULL);
		if (lqhpp[j] == NULL) {
			error ("error creating LQH");
		}
	}

	return (lqhpp);
}
