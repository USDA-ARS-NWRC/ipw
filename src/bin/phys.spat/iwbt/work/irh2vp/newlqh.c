/*
**
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, inverse, min, max)
**	int fdo;
**	bool_t inverse
**	double min, max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the irh2vp program.
** 
*/

#include "ipw.h"
#include "lqh.h"
#include "gethdrs.h"
#include "bih.h"
#include "pgm.h"

char *units_vp = "Pa";


void
newlqh (
	int             fdo,		/* output file desc		 */
	bool_t		inverse,	/* flag for inverse (VP->RH	 */
	double		min,		/* minimum RH or VP in output	 */
	double		max)		/* minimum RH or VP in output	 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[2];	/* integer break points	 */
	fpixel_t        fbkpt[2];	/* FP break points		 */
	char	       *units;		/* units of LQ header		 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0] = min;
	fbkpt[1] = max;
	if (min == max) fbkpt[1] += 1.0;
	if (inverse)
		units = (char *) NULL;
	else
		units = units_vp;

   /* create LQH */

	ibkpt[0] = 0;
	ibkpt[1] = ipow2 (hnbits(fdo, 0)) - 1;
	lqhpp[0] = lqhmake (hnbits(fdo, 0), 2, ibkpt, fbkpt, units, (char *) NULL);

   /* write new LQH */

	if (lqhwrite (fdo, lqhpp) == ERROR) {
		error ("error writing new LQH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}

}
