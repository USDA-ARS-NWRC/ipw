/*
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, svp_min, svp_max)
**	int fdo;
**	float svp_min;
**	float svp_max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW lq header for the output
**	image of program isatvp
** 
** RETURN VALUE
** 	0 if OK; ERROR if error
** 
*/

#include        <math.h>
#include        "ipw.h"
#include	"pgm.h"
#include	"lqh.h"
#include        "bih.h"
#include        "pixio.h"
#include	"gethdrs.h"
#include        "fpio.h"
#include        "envphys.h"


char *units[] = {
	"Pa",
	"Pa"
};


void
newlqh (
	int             fdo,		/* output file desc		 */
	float		svp_min,	/* min of svp (band 0)	 */
	float		svp_max)	/* max of svp (band 0)	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = svp_min;
	fbkpt[0][1] = svp_max;
	if (svp_max == svp_min) fbkpt[0][1] += 1.0;

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
