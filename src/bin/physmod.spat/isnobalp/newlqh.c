/*
** NAME
** 	newlqh -- create a new IPW LQH header for the output or intermediate
**		  output image of the snow melt model isnobal
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	newlqh(
**	    int         fdo,		|* output file desc		 *|
**	    int		nbands,		|* # bands in output image	 *|
**	    fpixel_t   *mins,		|* min value for each band	 *|
**	    fpixel_t   *maxs,		|* max value for each band	 *|
**	    char      **units)		|* units string for each band	 *|
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the snow melt model isnobal.
*/

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"
#include "pgm.h"

void
newlqh(
	int             fdo,		/* output file desc		 */
	int		nbands,		/* # bands in output image	 */
	fpixel_t       *mins,		/* min value for each band	 */
	fpixel_t       *maxs,		/* max value for each band	 */
	char          **units)		/* units string for each band	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[2];	/* integer break points	 */
	fpixel_t        fbkpt[2];	/* FP break points		 */
	int             j;		/* loop counter			 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (nbands, sizeof(LQH_T *), fdo, LQH_HNAME);

   /* create LQH for each band */

	for (j = 0; j < nbands; j++) {
		ibkpt[0] = 0;
		ibkpt[1] = ipow2 (hnbits(fdo, j)) - 1;
		fbkpt[0] = mins[j];
		fbkpt[1] = maxs[j];
		if (mins[j] == maxs[j]) {
			fbkpt[1] = maxs[j] + 1;
		}
		lqhpp[j] = lqhmake(hnbits(fdo, j), 2,
				ibkpt, fbkpt, units[j], (char *) NULL);
	}

   /* write new LQH */

	if (lqhwrite(fdo, lqhpp) == ERROR) {
		error("error writing new LQH");
	}

   /* prepare for image output */

	if (boimage(fdo) == ERROR) {
		error("can't terminate header output");
	}
}
