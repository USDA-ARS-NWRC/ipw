/*
** NAME
** 	fillends -- fill ends of line
** 
** SYNOPSIS
**	fillends (n, k)
** 	int n;
**	fpixel_t *k;
** 
** DESCRIPTION
**	 fill ends of line
** 
*/

#include "ipw.h"
#include "fpio.h"
#include "pgm.h"

void
fillends(
	int		n,		/* genuine # samples	 */
	fpixel_t       *k)		/* -> line		 */
{

   /* left end */

	k[-1] = k[0] + (k[0] - k[1]);

   /* right end */

	k[n] = k[n - 1] - (k[n - 2] - k[n - 1]);
}
