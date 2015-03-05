/*
**	convert Fortran D-format input to E-format
**	also make sure NULL at end of string
*/


#include <ctype.h>
#include <math.h>

#include "ipw.h"
#include "pgm.h"

double
bigatof(
	char           *s)
{
	register        j;
	register char  *p;
	char            temp[BIGFSIZ + 1];

	temp[BIGFSIZ] = '\0';

	p = temp;
	for (j = 0; j < BIGFSIZ; j++) {
		if (s[j] == 'D')
			*p++ = 'E';
		else
			*p++ = s[j];
	}

	return (atof(temp));
}
