/*
** NAME
** 	newlqh -- create new linear quantization header
** 
** SYNOPSIS
**	LQH_T ** newlqh (fdo, slope, aspect);
** 	int fdo;
**	bool_t slope, aspect;
** 
** DESCRIPTION
** 	newlqh creates a new linear quantization header for the ggradient 
**	program.
** 
*/

#include <math.h>
#include <string.h>
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "pgm.h"

LQH_T         **
newlqh(
	int             fdo,		/* output file desc		 */
	bool_t		slope,		/* flag for slope output	 */
	bool_t		aspect)		/* flag for aspect output	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	fpixel_t        fbkpt[2][2];	/* limits of LQ			 */
	pixel_t         ibkpt[2][2];	/* integer break points		 */
	char           *units[2];	/* units			 */
	int		nbands;		/* # bands in output image	 */
	int             j;


	nbands = hnbands (fdo);

   /* if slope to be computed */

	if (slope) {
		fbkpt[0][0] = 0;
		fbkpt[0][1] = 1;
		units[0] = NULL;
		if (aspect) {
			fbkpt[1][1] = M_PI *
				(1 - ldexp (1.0, 1 - hnbits(fdo, 1)));
			fbkpt[1][0] = -M_PI;
			units[1] = strdup("radians");
		}

   /* must be aspects only */

	} else if (aspect) {
		fbkpt[0][1] = M_PI *
			(1 - ldexp (1.0, 1 - hnbits(fdo, 0)));
		fbkpt[0][0] = -M_PI;
		units[0] = strdup("radians");
	}

   /* return the new LQH */

	ibkpt[0][0] = ibkpt[1][0] = 0;
	ibkpt[0][1] = ipow2 (hnbits(fdo, 0)) - 1;
	if (nbands > 1)
		ibkpt[1][1] = ipow2 (hnbits(fdo, 1)) - 1;

	lqhpp = (LQH_T **) hdralloc (nbands, sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	for (j = 0; j < nbands; j++) {
		lqhpp[j] = lqhmake (hnbits(fdo, j), 2,
				ibkpt[j], fbkpt[j], units[j], (char *) NULL);
	}

	return (lqhpp);
}
