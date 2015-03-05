/*
** NAME
** 	newcrh -- write the new CRH header for classeq
** 
** SYNOPSIS
**	newcrh (fdi, fdo, lo, hi, rep, nclass, annot)
**	int fdi, fdo;
** 	fpixel_t *lo, *hi, *rep;
**	int nclass;
**	char *annot;
** 
** DESCRIPTION
** 	newcrh allocates, initializes and writes a CRH header to the
**	output IPW image file.
*/

#include "ipw.h"
#include "crh.h"
#include "hdrio.h"
#include "lqh.h"

void
newcrh (
	int		 fdi,		/* input image file descriptor  */
	int		 fdo,		/* output image file descriptor */
	fpixel_t	*lo,		/* lo values for each class   	*/
	fpixel_t	*hi,		/* hi values for each class   	*/
	fpixel_t	*rep,		/* rep values for each class   	*/
	pixel_t		floor,		/* class for pixel < lowest    	*/
	pixel_t		ceil,		/* class for pixel > highest   	*/
	int		nclass,		/* # classes			*/
	char		*annot)		/* -> annotation for CRH 	*/
{
	CRH_T	       **o_crhpp;	/* -> new CRH			*/
	LQH_T	       **i_lqhpp;	/* -> LQH of input image	*/
	char		*units;		/* -> units of input LQH	*/


    /* allocate CRH */

	o_crhpp = (CRH_T **) hdralloc (1, sizeof(CRH_T *), fdo, CRH_HNAME);
	if (o_crhpp == NULL) {
		error ("Unable to allocate CRH");
	}

   /* get address of input LQH - to copy units */

	i_lqhpp = lqh(fdi);
	if (i_lqhpp == NULL)
		units = NULL;
	else
		units = lqh_units (i_lqhpp[0]);

   /* initialize CRH */

	o_crhpp[0] = crhmake (nclass, lo, hi, rep, floor, ceil, annot, units);
	if (o_crhpp[0] == NULL) {
		error ("Unable to make CRH");
	}

   /* write CRH */

	if (crhwrite (fdo, o_crhpp) == ERROR) {
		error ("cannot write CRH");
	}
}
