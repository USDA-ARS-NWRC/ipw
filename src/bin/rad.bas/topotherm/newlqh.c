/*
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, lw_min, lw_max)
**	int fdo;
**	float lw_min;
**	float lw_max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of thermal radiation model topotherm
** 
** RETURN VALUE
** 	0 if OK; ERROR if error
** 
*/

#include        <math.h>
#include        "ipw.h"
#include	"lqh.h"
#include        "bih.h"
#include        "pixio.h"
#include	"gethdrs.h"
#include        "fpio.h"
#include        "envphys.h"
#include	"pgm.h"


char *units[] = {
	"wm2",
	"wm2"
};


void
newlqh (
	int             fdo,		/* output file desc		 */
	float		lw_min,		/* min of lw_in (band 0)	 */
	float		lw_max)		/* max of lw_in (band 0)	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */

   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = lw_min;
	fbkpt[0][1] = lw_max;
	if (lw_max == lw_min) fbkpt[0][1] += 1.0;

   /* create LQH for band 0 */

	ibkpt[0][0] = 0;
	ibkpt[0][1] = ipow2 (hnbits(fdo, 0)) - 1;
	lqhpp[0] = lqhmake (hnbits(fdo, 0), 2,
			ibkpt[0], fbkpt[0], units[0], (char *) NULL);

   /* write new LQH */

	if (lqhwrite (fdo, lqhpp) == ERROR) {
		error ("error writing new LQH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}
}
