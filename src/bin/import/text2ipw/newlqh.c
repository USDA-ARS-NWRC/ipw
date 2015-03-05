/******************************************************************************
**
** NAME
** 	newlqh -- create a new IPW LQH header for the iimport program
** 
** SYNOPSIS
**	void newlqh (fdo, nbands, min, max);
**	int fdo;
**	int nbands;
**	float *min, *max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes a LQH header for the output
**	image of the iimport program.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"

void
newlqh (
	int             fdo,		/* output file desc		 */
	int		nbands,		/* # bands in output image	 */
	float	       *min,		/* minimum value for each band	 */
	float	       *max)		/* maximum value for each band	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t		ibkpt[2];	/* integer break points		 */
	fpixel_t	fbkpt[2];	/* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (nbands, sizeof(LQH_T *), fdo, LQH_HNAME);
	if (lqhpp == NULL) {
		error ("can't allocate LQH header");
	}

   /* create LQH for each band */

	ibkpt[0] = 0;

	for (j = 0; j < nbands; j++) {
		ibkpt[1] = ipow2(hnbits(fdo, j)) - 1;
		fbkpt[0] = min[j];
		if (min[j] == max[j])
			fbkpt[1] = min[j] + 1.0;	/* fudge if min=max */
		else
			fbkpt[1] = max[j];
		lqhpp[j] = lqhmake (hnbits(fdo, j), 2,
				ibkpt, fbkpt, (char *) NULL, (char *) NULL);
		if (lqhpp[j] == NULL) {
			error ("can't make LQH for band %d", j);
		}
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
