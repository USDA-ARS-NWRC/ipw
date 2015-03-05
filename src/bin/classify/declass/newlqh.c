/*
**
** NAME
** 	newlqh -- create a new IPW LQH header
** 
** SYNOPSIS
**	LQH_T **newlqh (nbands, nbits, crhpp, fdo)
**	int nbands;
**	int *nbits;
**	CRH_T **crhpp;
**	int fdo;
** 
** DESCRIPTION
** 	newlqh allocates and initializes an IPW header for the output image
**	of the declass program.
** 
** RETURN VALUE
** 	pointer to new LQ header; NULL if error
*/

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "lqh.h"
#include "crh.h"
#include "pgm.h"


LQH_T         **
newlqh(
	int		nbands,		/* # bands in output image	 */
	int	       *nbits,		/* # bits in each band		 */
	CRH_T         **crhpp,		/* -> input image CRH		 */
	int             fdo)		/* output file desc		 */
{
	LQH_T         **lqhpp;		/* -> output LQH		 */
	pixel_t         ibkpt[2];	/* integer break points		 */
	fpixel_t        fbkpt[2];	/* floating point break points	 */
	int             b;		/* band loop counter		 */


   /* allocate LQH */

	lqhpp = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	if (lqhpp == NULL) {
		error ("can't allocate new LQH");
	}

   /* create LQH for each band */

	ibkpt[0] = 0;
	for (b = 0; b < nbands; b++) {
		ibkpt[1] = ipow2 (hnbits(fdo, b)) - 1;
		fbkpt[0] = crh_lo (crhpp[b], 0);
		fbkpt[1] = crh_hi (crhpp[b], crh_nclass(crhpp[b])-1);
		lqhpp[b] = lqhmake (hnbits(fdo, b), 2,
				ibkpt, fbkpt, crh_units(crhpp[b]),
				(char *) NULL);
	}

   /* return new LQH */

	return (lqhpp);
}
