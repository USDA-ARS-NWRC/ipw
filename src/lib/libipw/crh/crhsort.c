/*
** NAME
**	crhsort -- return a copy of classes sorted by class number
**
** SYNOPSIS
**	#include "crh.h"
**
**	int
**	crhsort(
**		CRH_T   *crhp,		|* -> CRH for a specific band	    *|
**		int	 sortkey)	|* = SORT_BY_CLASS or SORT_BY_RANGE *|
**
** DESCRIPTION
**      crhsort sorts the given CRH header according to the given sort key:
**	SORT_BY_CLASS or SORT_BY_RANGE.
**
**	crhsort is called by IPW application programs to sort the CRH
**	header by class number or by range.
**
** RETURN VALUE
**	OK for success, ERROR for failure
*/

#include "ipw.h"
#include "crh.h"

static int compar_cls(CLASS *class1, CLASS *class2);
static int compar_rng(CLASS *class1, CLASS *class2);

int
crhsort(
	CRH_T         *crhp,		/* -> CRH for a specific band	    */
	int	       sortkey)		/* = SORT_BY_CLASS or SORT_BY_RANGE */
{

	switch (sortkey) {

		case SORT_BY_CLASS:
			(void) SORT_ALG(crhp->class, crhp->nclass,
				sizeof(CLASS), compar_cls);
			break;

		case SORT_BY_RANGE:
			(void) SORT_ALG(crhp->class, crhp->nclass,
				sizeof(CLASS), compar_rng);
			break;
		default:
			usrerr ("illegal key for crhsort");
			return (ERROR);
	}

	return (OK);

}

/* ------------------------------------------------------------------------ */

static int
compar_cls(
	CLASS *class1,
	CLASS *class2)
{
	if (class1->cls >= class2->cls)
		return (1);
	else
		return (-1);
}

/* ------------------------------------------------------------------------ */

static int
compar_rng(
	CLASS *class1,
	CLASS *class2)
{
	if (class1->rep >= class2->rep)
		return (1);
	else
		return (-1);
}
