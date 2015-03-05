/*
**
** name
** 	newlqh -- create a new IPW LQH header
** 
** synopsis
**	void newlqh (fdo, mins, maxs);
**	int fdo;	{ file descriptor for output image		}
**	double *mins;	{ array of minimums for bands in output image	}
**	double *maxs;	{ array of maximums for bands in output image	}
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
	"W m^2",
	"W m^2",
	"W m^2",
	"s/m"
};


void
newlqh(
	int             fdo,		/* output file desc		 */
	double	       *mins,		/* array of minimums for bands	 */
	double	       *maxs)		/* array of maximums for bands 	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	for (j = 0; j < OBANDS; j++) {
		fbkpt[j][0] = mins[j];
		fbkpt[j][1] = maxs[j];
		if (mins[j] == maxs[j])
			fbkpt[j][1] += 1.0;
	}

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
