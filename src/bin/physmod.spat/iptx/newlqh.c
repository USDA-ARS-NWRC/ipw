/*
**
** name
** 	newlqh -- create a new IPW LQH header
** 
** synopsis
**	void newlqh (fdo, le_min, le_max);
**	int fdo;
**	double le_min, le_max;
** 
** description
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image.
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "pgm.h"

char *units[] = {
	"W m^2"
};


void
newlqh(
	int            fdo,		/* output file desc		 */
	double	       le_min,		/* min le 			 */
	double	       le_max)		/* max le			 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = le_min;
	fbkpt[0][1] = le_max;
	if (le_min == le_max)
		fbkpt[0][1] += 1.0;

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
