/*
** NAME
**      _bihfree -- free a BIH header for a image file descriptor
**
** SYNOPSIS
**      #include "_bih.h"
**
**	void
**	_bihfree(
**		int	fd)		|* image file descriptor	*|
**
** DESCRIPTION
**      _bihfree frees any BIH structures that might be left over after a file
**	is closed.
*/

#include "ipw.h"
#include "_bih.h"

void
_bihfree(
	int	fd)		/* image file descriptor	*/
{
	BIH_T **bihpp;
	int 	band;
	int	nbands;

	bihpp = bih(fd);
	if (bihpp == NULL)
		return;

	nbands = bih_nbands(bihpp[0]);

	SAFE_FREE (bihpp[0]->img->byteorder);
	SAFE_FREE (bihpp[0]->img);

	for (band = 0; band < nbands; band++) {
		if (bihpp[band]->annot != NULL) {
			while ( (bihpp[band]->annot->n != 0)       &&
			        (bihpp[band]->annot->v[0] != NULL) )
			    delsv (bihpp[band]->annot, 0);
			SAFE_FREE(bihpp[band]->annot);
		}
		if (bihpp[band]->history != NULL) {
			while ( (bihpp[band]->history->n != 0)       &&
			        (bihpp[band]->history->v[0] != NULL) )
			    delsv (bihpp[band]->history, 0);
			SAFE_FREE(bihpp[band]->history);
		}
		SAFE_FREE (bihpp[band]);
	}
	SAFE_FREE(bihpp);
	_bih[fd] = NULL;
}
