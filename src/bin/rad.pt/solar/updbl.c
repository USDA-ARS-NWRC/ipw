/*
**  NAME
**	updbl, downdbl - functions for sorting
**
**	downdbl - integer function for downward sorting
**	updbl - integer function for upward sorting
**
**  SYNOPSIS
**	int	downdbl(x, y)
**	double	*x, *y;
**
**	int	updbl(x, y)
**	double	*x, *y;
**
**  DESCRIPTION
**	integer function for downward or upward sorting, to be combined
**	with qsort.
**	downdbl and updbl are for sorting double's, equivalent functions
**	for other types can be easily made.
**
**  SEE ALSO
**	qsort(3) in UNIX manual
*/

#include "ipw.h"
#include "pgm.h"

int
updbl(
	double         *x,
	double         *y)
{
	int             result;

	result = *x != *y;

	if (result && *x < *y)
		result = -1;

	return (result);
}

int
downdbl(
	double         *x,
	double         *y)
{
	int             result;

	result = *x != *y;

	if (result && *x > *y)
		result = -1;

	return (result);
}
