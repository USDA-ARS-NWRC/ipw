/* revgen - reverse vector with elements of arbitrary length */

/*
 *	First reverse the entire vector, byte by byte.  Now each of the
 *	elements also have their bytes reverse.  Reverse these back
 *	again.
 */

#include "ipw.h"

#include "pgm.h"

void
revgen(
	register addr_t	a,		/*  -> beginning of data	*/
	int		nelem,		/*  # elements in vector	*/
	register int	nb)		/*  # bytes per element		*/
{
	register int	j;

	revchar(a, nelem*nb);
	j = nelem;
	while (--j >= 0) {
		revchar(a, nb);
		a = (char *) a + nb;
	}
}
