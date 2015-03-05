/*
 * create new LQH
 */

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"

LQH_T **
newlqh(
	int             fdi,		/* input file descriptor	 */
	int             fdo)		/* output file descriptor	 */
{
	LQH_T         **lqhp;		/* -> output LQH		 */
	char            units[32];	/* units			 */
	pixel_t         ival[2];
	fpixel_t        fval[2];

	(void) sprintf(units, "cosZ");
	ival[0] = fval[0] = 0;
	ival[1] = ipow2(hnbits(fdi, 0)) - 1;
	fval[1] = 1;

 /* NOSTRICT */
	lqhp = (LQH_T **) hdralloc(1, sizeof(LQH_T *), fdo, LQH_HNAME);
	assert(lqhp != NULL);
	lqhp[0] = lqhmake(hnbits(fdi, 0), 2, ival, fval, units, (char *) NULL);

	return (lqhp);
}
