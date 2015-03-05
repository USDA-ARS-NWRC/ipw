/*
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, vmin, vmax, irmin, irmax, vis_only, ir_only)
**	int fdo;
**	double vmin, vmax, irmin, irmax;
**	bool_t vis_only, ir_only;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the snow albedo program ialbedo.
** 
*/

#include "ipw.h"
#include "lqh.h"
#include "bih.h"
#include "pgm.h"
#include "gethdrs.h"


void
newlqh (
	int             fdo,		/* output file desc		 */
	double		vmin,		/* min of alb(vis) (band 0)	 */
	double		vmax,		/* max of alb(vis) (band 0)	 */
	double		irmin,		/* min of alb(ir) (band 1)	 */
	double		irmax,		/* max of alb(ir) (band 1)	 */
	bool_t		vis_only,	/* output visible band only	 */
	bool_t		ir_only)	/* output IR band only		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[2][2];	/* integer break points	 */
	fpixel_t        fbkpt[2][2];	/* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	if (ir_only) {
		fbkpt[0][0] = irmin;
		fbkpt[0][1] = irmax;
	} else {
		fbkpt[0][0] = vmin;
		fbkpt[0][1] = vmax;
	}
	if (!vis_only) {
		fbkpt[1][0] = irmin;
		fbkpt[1][1] = irmax;
	}

   /* create LQH for each band */

	for (j = 0; j < hnbands(fdo); j++) {
		ibkpt[j][0] = 0;
		ibkpt[j][1] = ipow2 (hnbits(fdo, j)) - 1;
		lqhpp[j] = lqhmake (hnbits(fdo, j), 2,
				ibkpt[j], fbkpt[j], (char *) NULL, (char *) NULL);
	}

   /* write new LQH */

	if (lqhwrite (fdo, lqhpp) == ERROR) {
		error ("error writing new LQH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}
}
