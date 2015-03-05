/*
** NAME
**	_crhcheck -- validate components of IPW CRH header
**
** SYNOPSIS
**	#include "crh.h"
**
**	bool_t
**	_crhcheck(
**		CRH_T **crhpp,		|* -> array of -> CRH header	 *|
**		int     nbands)		|* # header bands		 *|
**
** DESCRIPTION
**	_crhcheck checks that crhpp points to an array of nbands pointers
**	to valid CRH headers.
**
** RETURN VALUE
**	TRUE	if crhpp checks OK
**
**	FALSE	class-range header is not valid; message stored via 'usrerr'
*/

#include "ipw.h"
#include "_crh.h"

static int compar(CLASS *class1, CLASS *class2);

bool_t
_crhcheck(
	CRH_T         **crhpp,		/* -> array of -> CRH header	 */
	int             nbands)		/* # header bands		 */
{
	int             band;		/* loop counter			 */
	bool_t          found;		/* ? found at least 1 header	 */


	assert (crhpp != NULL);

   /* loop through possible bands */

	found = FALSE;

	for (band = 0; band < nbands; ++band) {
		int             i;	/* loop counter			 */
		CRH_T          *crhp;	/* -> CRH for current band	 */

		crhp = crhpp[band];
		if (crhp == NULL) {
			continue;
		}

		found = TRUE;

		assert (crhp->nclass > 0);
		assert (crhp->class != NULL);

		/* sort by class ranges */

		(void) SORT_ALG(crhp->class, crhp->nclass, sizeof(CLASS),
			      compar);

		for (i = 0; i < crhp->nclass; i++) {

			/* assure that representative value is inside range */

			if (crhp->class[i].rep < crhp->class[i].lo ||
			    crhp->class[i].rep >= crhp->class[i].hi) {
				usrerr (
				       "\"%s\" header, band %d: illegal rep value",
				       CRH_HNAME, band);
				return (FALSE);
			}

			/* assure that classes are sorted and do not overlap */

			if (i > 0)
			   if (crhp->class[i].lo < crhp->class[i-1].hi) {
				usrerr (
				       "\"%s\" header, band %d: classes unsorted or overlapping",
				       CRH_HNAME, band);
				return (FALSE);
			}
		}
	}

	assert(found);

   /* done - must be OK */

	return (TRUE);
}

/* ------------------------------------------------------------------------ */

static int
compar(
	CLASS *class1,
	CLASS *class2)
{
	/* sort by lo values */
	if (class1->lo >= class2->lo)
		return(1);
	else
		return(-1);
}
