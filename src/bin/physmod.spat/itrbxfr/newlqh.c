/******************************************************************************
**
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, h_min, h_max, le_min, le_max, mm_min, mm_max)
**	int fdo;
**	double h_min, h_max;
**	double le_min, le_max;
**	double mm_min, mm_max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the turbulent transfer model itrbxfr.
** 
** RETURN VALUE
** 	0 if OK; ERROR if error
** 
** HISTORY
**	Aug 1995	Changed definition of ibkpt and fbkpt from IBANDS
**			to OBANDS.  J. Domingo, OSU, EPA/NHEERL/WED
** 
*/

#include "ipw.h"
#include "bih.h"
#include "pgm.h"
#include "gethdrs.h"

char *units[] = {
	"W/m^2",
	"W/m^2",
	"mm/m^2 s^-1"
};


void
newlqh (
	int             fdo,		/* output file desc		 */
	double		h_min,		/* min of h (band 0)		 */
	double		h_max,		/* max of h (band 0)		 */
	double		le_min,		/* min of le (band 1)		 */
	double		le_max,		/* max of le (band 1)		 */
	double		mm_min,		/* min of mm (band 2)		 */
	double		mm_max)		/* max of mm (band 2)		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = h_min;
	fbkpt[0][1] = h_max;
	fbkpt[1][0] = le_min;
	fbkpt[1][1] = le_max;
	fbkpt[2][0] = mm_min;
	fbkpt[2][1] = mm_max;

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
