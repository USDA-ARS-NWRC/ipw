/*
**	reverse a vector: call either specialized or general routine
**	depending on size of each element
*/

#include "ipw.h"
#include "pgm.h"

void
reverse(
	addr_t          a,		/* -> beginning of data		 */
	int             nelem,		/* # elements in vector 	 */
	int             nb)		/* # bytes per element		 */
{
	if (nelem > 1 && nb >= 1) {
		if (nb == sizeof(char)) {
			revchar((char *) a, nelem);
		}
		else if (nb == sizeof(short)) {
 /* NOSTRICT */
			revshort((short *) a, nelem);
		}
		else if (nb == sizeof(long)) {
 /* NOSTRICT */
			revlong((long *) a, nelem);
		}
		else {
			revgen(a, nelem, nb);
		}
	}
}
