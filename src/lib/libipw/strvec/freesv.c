/*
** NAME
**	freesv -- free an entire strvec
**
** SYNOPSIS
**	#include "ipw.h"
**
**	int
**	freesv(
**		STRVEC_T *p)
**
** DESCRIPTION
**	freesv frees the entire strvec structure.
**
** RETURN VALUE
**	OK is the free was successful, ERROR otherwise.
*/

#include "ipw.h"
#include "_strvec.h"

int
freesv(
	STRVEC_T *p)
{
	int i;

	if ( p == NULL)
		return (OK);

	assert(_ok_sv(p));

	for (i = 0; i < p->n; i++) {
		SAFE_FREE(p->v[i]);
	}
	SAFE_FREE(p->v);
	SAFE_FREE(p);

	return (OK);
}
