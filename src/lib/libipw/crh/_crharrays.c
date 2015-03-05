/*
** NAME
**	_crharrays -- allocate arrays for CR header
**
** SYNOPSIS
**	#include "_crh.h"
**
**	int
**	_crharrays(
**		CRH_T    *crhp)		|* -> CR header			 *|
**
** DESCRIPTION
**	_crharrays allocates the arrays required by the given class-range
**	header.
**
**	crhp->nclass must be set before _crharrays is called.
**
** RETURN VALUE
**	OK	arrays successfully allocated.
**
**	ERROR	insufficient memory for arrays
*/

#include "ipw.h"
#include "_crh.h"

int
_crharrays(
	CRH_T          *crhp)		/* -> CR header			 */
{
	assert(crhp != NULL);
	assert(crhp->nclass > 0);

	crhp->class = (CLASS *) ecalloc(crhp->nclass, sizeof(CLASS));
	if (crhp->class == NULL) {
		return (ERROR);
	}

	return (OK);
}
