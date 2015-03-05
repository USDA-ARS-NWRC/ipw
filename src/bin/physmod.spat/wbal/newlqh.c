/*
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	void newlqh (fdo, ro_min, ro_max, s_min, s_max, et_min, et_max)
**	int fdo;
**	float ro_min, ro_max;
**	float s_min, s_max;
**	float et_min, et_max;
** 
** DESCRIPTION
** 	newlqh allocates, initializes, and writes an IPW header for the output
**	image of the simple water balance model wbal
** 
** RETURN VALUE
** 	0 if OK; ERROR if error
** 
*/

#include "ipw.h"
#include "lqh.h"
#include "pgm.h"
#include "gethdrs.h"
#include "bih.h"

char *units[] = {
	"mm",
	"mm"
};


void
newlqh (
	int             fdo,		/* output file desc		 */
	float		ro_min,		/* min of runoff (band 0)	 */
	float		ro_max,		/* max of runoff (band 0)	 */
	float		s_min,		/* min of storage (band 1)	 */
	float		s_max,		/* max of storage (band 1)	 */
	float		et_min,		/* min of et (band 2)		 */
	float		et_max)		/* max of et (band 2)		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[OBANDS][2]; /* integer break points	 */
	fpixel_t        fbkpt[OBANDS][2]; /* FP break points		 */
	int             j;


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);

	fbkpt[0][0] = ro_min;
	fbkpt[0][1] = ro_max;
	if (ro_max == ro_min) fbkpt[0][1] += 1.0;
	fbkpt[1][0] = s_min;
	fbkpt[1][1] = s_max;
	if (s_max == s_min) fbkpt[1][1] += 1.0;
	fbkpt[2][0] = et_min;
	fbkpt[2][1] = et_max;
	if (et_max == et_min) fbkpt[2][1] += 1.0;

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
