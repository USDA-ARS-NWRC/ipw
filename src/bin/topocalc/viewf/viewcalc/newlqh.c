/*
 * create new LQH
 */

#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"
#include "pgm.h"

LQH_T         **
newlqh(
	int             fdi,		/* input file descriptor	 */
	int             fdo)		/* output file descriptor	 */
{
	LQH_T         **lqhp;		/* -> output LQH		 */
	char            units[32];	/* units			 */
	pixel_t         ival[2];
	fpixel_t        fval[2];	/* ranges for sky view factor	 */
	fpixel_t        tval[2];	/* ranges for terrain config
					 * fac */

	(void) sprintf(units, "sky view factor");
	ival[0] = 0;
	ival[1] = ipow2(hnbits(fdi, 0)) - 1;
	fval[0] = 0;
	fval[1] = 1;
	tval[0] = 1;
	tval[1] = 0;

 /* NOSTRICT */
	lqhp = (LQH_T **) hdralloc(2, sizeof(LQH_T *), fdo, LQH_HNAME);
	assert(lqhp != NULL);
	lqhp[0] = lqhmake(hnbits(fdi, 0), 2, ival, fval, units, (char *) NULL);

	(void) sprintf(units, "terrain config factor");
	lqhp[1] = lqhmake(hnbits(fdi, 0), 2, ival, tval, units, (char *) NULL);

	return (lqhp);
}
