/*
** NAME
** 	newlqh -- create new linear quantization header
** 
** SYNOPSIS
**	LQH_T ** newlqh (fdo);
** 	int fdo;
** 
** DESCRIPTION
** 	newlqh creates a new linear quantization header for the ghorizon 
**	program.
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "pgm.h"

LQH_T         **
newlqh(
	int             fdo)		/* output file desc		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	fpixel_t        fbkpt[2];	/* limits of LQ			 */
	pixel_t         ibkpt[2];	/* integer break points		 */
	char            units[32];	/* units			 */


   /* preamble, we use reverse quantization so that dark pixels are those with
    * large horizon angles, i.e. they are shaded */

	(void) sprintf(units, "cos H");

	fbkpt[0] = 1;
	fbkpt[1] = 0;
	ibkpt[0] = 0;
	ibkpt[1] = ipow2(hnbits(fdo, 0)) - 1;

   /* return the new LQH */

	lqhpp = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	lqhpp[0] = lqhmake (hnbits(fdo, 0), 2, ibkpt, fbkpt,
				   units, (char *) NULL);

	return (lqhpp);
}
