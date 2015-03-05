/*
** NAME
**      _lqhfree -- free all left over parts of a LQH header
**
** SYNOPSIS
**      #include "_lqh.h"
**
**	int
**	_lqhfree(
**		int      fd)		|* image file descriptor	*|
**
** DESCRIPTION
**      lqhfree frees any LQH structures that might be left over after a
**	file is closed.
*/

#include "ipw.h"
#include "_lqh.h"
#include "bih.h"

void
_lqhfree(
	int      fd)		/* image file descriptor	*/
{
	LQH_T	**lqhpp;
	int 	  nbands;
	int 	  band;

	lqhpp = lqh(fd);

	if (lqhpp != NULL) {
		nbands = hnbands(fd);
		for (band = 0; band < nbands; band++) {
			LQH_T  *lqhp;

			lqhp = lqhpp[band];
			if (lqhp == NULL)
				continue;

			SAFE_FREE(lqhp->units);
			SAFE_FREE(lqhp->interp);
			SAFE_FREE(lqhp->bkpt);
			SAFE_FREE(lqhp);
			/* lininv and map are freed by fpclose */
		}

		SAFE_FREE (lqhpp);
	}

	_lqh[fd] = NULL;
}
