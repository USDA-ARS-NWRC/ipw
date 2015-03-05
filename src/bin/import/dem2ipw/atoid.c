/*
**	just like atoi(), but make sure NULL at end of string and limited
**	to 6 characters
*/

#include "ipw.h"

#include "pgm.h"

int
atoid(
	char           *s)
{
	register        j;
	register char  *p;
	char            temp[ISIZE + 1];

	temp[ISIZE] = '\0';

	p = temp;
	for (j = 0; j < ISIZE; j++) {
		*p++ = *s++;
	}

	return (atoi(temp));
}
