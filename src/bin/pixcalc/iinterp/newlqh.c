/*
**
** NAME
** 	newlqh -- create a new IPW LQH header for iinterp output image
** 
** SYNOPSIS
**	void newlqh (fdo, min, max)
**	int fdo;
**	fpixel_t min, max;
** 
** DESCRIPTION
** 	newlqh allocates and initializes an IPW header for the output image
**	of the iinterp program.  The LQ header is written to the output
**	image and the image is prepared for image output.
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "pgm.h"


void
newlqh (
	int             fdo,		/* output file desc		 */
	fpixel_t	min,		/* min F.P. value for LQ map	 */
	fpixel_t	max)		/* max F.P. value for LQ map	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[2];	/* integer break points		 */
	fpixel_t        fbkpt[2];	/* floating point break points	 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	if (lqhpp == NULL) {
		error ("can't allocate new LQH");
	}

   /* create single-band LQH */

	ibkpt[0] = 0;
	ibkpt[1] = ipow2 (hnbits(fdo, 0)) - 1;
	fbkpt[0] = min;
	fbkpt[1] = max;
	if (min == max) fbkpt[1] += 1.0;

	lqhpp[0] = lqhmake (hnbits(fdo, 0), 2,
			ibkpt, fbkpt, (char *) NULL, (char *) NULL);

   /* write the new LQ header */

	if (lqhwrite (fdo, lqhpp) == ERROR) {
		error ("error writing LQ header");
	}

   /* terminate header output */

	if (boimage (fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
