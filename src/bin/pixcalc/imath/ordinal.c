#include "ipw.h"
#include "pgm.h"

char *
ordinal(
	int	i)
{
	assert(i >= 0);

	if ((i % 10 == 1) && (i % 100 != 11))
		return "st";
	else if ((i % 10 == 2) && (i % 100 != 12))
		return "nd";
	else if ((i % 10 == 3) && (i % 100 != 13))
		return "rd";
	else
		return "th";
}
