/*
**	modprod - finds (a b) mod i, protecting against overflow
**	(Bruce Allen, UCSB Physics)
*/

#include "ipw.h"
#include "pgm.h"

int
modprod(
	int	a,
	int	b,
	int	i)
{
	register	sum;

	for (sum = 0; ; ) {
		if (a & 01) {
			sum += b;
			sum %= i;
		}
		if ((a >>= 1) == 0)
			break;
		b <<= 1;
		b %= i;
	}

	return(sum);
}
