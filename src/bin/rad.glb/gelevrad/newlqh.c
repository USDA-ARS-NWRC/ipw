/*
**
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, bval, dval);
**	int fdo;
**	double *bval, *dbal;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the gelevrad model.
** 
** HISTORY
**	Aug 1995	Changed definition of ibkpt and fbkpt from IBANDS
**			to OBANDS.  J. Domingo, OSU, EPA/NHEERL/WED
** 
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "pgm.h"

char *units[] = {
	"W m^2",
	"W m^2",
};


void
newlqh (
	int             fdo,		/* output file desc		 */
	double	       *bval,		/* min/max beam radiation	 */
	double	       *dval)		/* min/max diffuse radiation	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = bval[0];
	fbkpt[0][1] = bval[1];
	if (bval[0] == bval[1])
		fbkpt[0][1] += 1.0;
	fbkpt[1][0] = dval[0];
	fbkpt[1][1] = dval[1];
		fbkpt[1][1] += 1.0;

   /* create LQH for each band */

	for (j = 0; j < hnbands(fdo); j++) {
		ibkpt[j][0] = 0;
		ibkpt[j][1] = ipow2 (hnbits(fdo, j)) - 1;
		lqhpp[j] = lqhmake (hnbits(fdo, j), 2,
				ibkpt[j], fbkpt[j], units[j], (char *) NULL);
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
