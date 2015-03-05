/*
** NAME
**	_pixioinit -- initialize IPW pixel I/O system
**
** SYNOPSIS
**	#include "_pixio.h"
**
**	void
**	_pixioinit(void)
**
** DESCRIPTION
**	_pixioinit() initializes I/O data structures for IPW pixels.
**	Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_piocb
*/

#include "ipw.h"
#include "_pixio.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_pixioinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 *  Initialize array of pointers to PIXIO control blocks.
	 */
	_piocb = (PIXIO_T **) ecalloc(_ipwmaxfd + 1, sizeof(PIXIO_T *));
	if (_piocb == NULL)
	    error("Insufficient memory for IPW pixel I/O system");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _piocb[i] = NULL;
}
