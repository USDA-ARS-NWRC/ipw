/*
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, dpt_min, dpt_max)
**	int fdo;
**	float dpt_min;
**	float dpt_max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW lq header for the output
**	image of program idewpt
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
	"C",
	"C"
};


void
newlqh (
	int             fdo,		/* output file desc		*/
	float		dpt_min,	/* min of dpt (band 0)		*/
	float		dpt_max)	/* max of dpt (band 0)		*/
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = dpt_min;
	fbkpt[0][1] = dpt_max;
	if (dpt_max == dpt_min) fbkpt[0][1] += 1.0;

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
