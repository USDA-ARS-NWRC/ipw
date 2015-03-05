/*
** NAME
**	crhmake -- make an IPW CRH header
**
** SYNOPSIS
**	#include "crh.h"
**
**	CRH_T *
**	crhmake(
**		int         nclass,	|* #classes     		 *|
**		fpixel_t   *lo,		|* array of lower range values   *|
**		fpixel_t   *hi,		|* array of higher range values  *|
**		fpixel_t   *rep,	|* array of representative vals  *|
**		pixel_t	    floor,	|* class floor value		 *|
**		pixel_t	    ceil,	|* class ceiling value		 *|
**		char       *annot,	|* annotation                    *|
**		char       *units)	|* units of data                 *|
**
** DESCRIPTION
**	crhmake allocates a single IPW CR header.  The header is initialized
**	with the arguments.
**
** RETURN VALUE
**	pointer to new CR header; NULL if error
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_crh.h"

CRH_T *
crhmake(
	int             nclass,		/* #classes     		 */
	fpixel_t       *lo,		/* array of lower range values   */
	fpixel_t       *hi,		/* array of higher range values  */
	fpixel_t       *rep,		/* array of representative vals  */
	pixel_t		floor,		/* class floor value		 */
	pixel_t		ceil,		/* class ceiling value		 */
	char           *annot,		/* annotation                    */
	char           *units)		/* units of data                 */
{
	CRH_T          *crhp;		/* -> CR header			 */
	int             cls;		/* current class #     		 */

	assert(nclass > 0);
	assert(lo != NULL);
	assert(hi != NULL);
	assert(rep != NULL);

   /* allocate header */

	crhp = (CRH_T *) hdralloc(1, sizeof(CRH_T), ERROR, CRH_HNAME);
	if (crhp == NULL) {
		return (NULL);
	}

   /* initialize scalars */

	crhp->nclass = nclass;
	crhp->floor = floor;
	crhp->ceil = ceil;

   /* initialize strings */

	crhp->annot = NULL;
	if (annot != NULL && (crhp->annot = strdup(annot)) == NULL) {
		return (NULL);
	}

	crhp->units = NULL;
	if (units != NULL && (crhp->units = strdup(units)) == NULL) {
		return (NULL);
	}


   /* initialize arrays */

	if (_crharrays(crhp) == ERROR) {
		return (NULL);
	}

	for (cls = 0; cls < nclass; cls++) {

		crhp->class[cls].cls = cls + 1;
		crhp->class[cls].lo = lo[cls];
		crhp->class[cls].hi = hi[cls];
		crhp->class[cls].rep = rep[cls];
	}

	return (_crhcheck(&crhp, 1) ? crhp : NULL);
}
