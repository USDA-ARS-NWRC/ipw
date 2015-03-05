/*
** NAME
**	crhdup -- duplicate an IPW CRH header
**
** SYNOPSIS
**	#include "crh.h"
**
**	CRH_T **
**	crhdup(
**		CRH_T **oldhpp,		|* -> old CRH array		 *|
**		int     nbands)		|* # header bands		 *|
**
** DESCRIPTION
**	crhdup creates a duplicate of the IPW CRH header pointed to by
**	"oldhpp".
**
**	crhdup is an easy way for applications programs to create a new CRH
**	(e.g., for an output image, after having read the CRH for an input
**	image).
**
** RETURN VALUE
**	A pointer to the duplicate header is returned.  NULL is returned if
**	an error occurs.
*/

#include <string.h>

#include "ipw.h"
#include "hdrio.h"
#include "_crh.h"

CRH_T **
crhdup(
	CRH_T         **oldhpp,		/* -> old CRH array		 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	CRH_T         **newhpp;		/* -> new CRH array		 */


   /* source CRH must be valid */

	assert (_crhcheck(oldhpp, nbands));

   /* allocate new pointer array */

	newhpp = (CRH_T **) hdralloc (nbands, sizeof(CRH_T *), ERROR,
			CRH_HNAME);
	if (newhpp == NULL) {
		return (NULL);
	}

   /* duplicate headers */

	for (band = 0; band < nbands; ++band) {
		CRH_T          *newhp;	/* -> new CRH			 */
		CRH_T          *oldhp;	/* -> new CRH			 */

		oldhp = oldhpp[band];
		newhp = (CRH_T *) hdralloc(1, sizeof(CRH_T), ERROR,
					   CRH_HNAME);
		if (newhp == NULL) {
			return (NULL);
		}

   /* duplicate scalar fields */

		bcopy((void *) oldhp, (void *) newhp, sizeof(CRH_T));

   /* duplicate string fields */

		if (oldhp->annot != NULL
		    && (newhp->annot = strdup(oldhp->annot)) == NULL) {
			return (NULL);
		}

		if (oldhp->units != NULL
		    && (newhp->units = strdup(oldhp->units)) == NULL) {
			return (NULL);
		}
 
   /* duplicate arrays */

		if (_crharrays(newhp) == ERROR) {
			return (NULL);
		}

		(void) bcopy((void *) newhp->class, (void *) oldhp->class,
			      (size_t) newhp->nclass * sizeof(CLASS));
 
   /* done with this band's header */

		newhpp[band] = newhp;
	}

	return (newhpp);
}
