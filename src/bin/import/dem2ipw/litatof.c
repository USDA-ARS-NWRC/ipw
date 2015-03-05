/*
**	just like atof(), but make sure NULL at end of string
*/

#include <math.h>

#include "ipw.h"

#include "pgm.h"

double
litatof(
	char           *s)
{
	register        j;
	register char  *p;
	char            temp[LITFSIZ + 1];

	temp[LITFSIZ] = '\0';

	p = temp;
	for (j = 0; j < LITFSIZ; j++) {
		*p++ = *s++;
	}

	return (atof(temp));
}
