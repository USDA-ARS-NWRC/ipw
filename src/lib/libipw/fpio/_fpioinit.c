/*
** NAME
**	_fpioinit -- initialize IPW floating-point I/O system
**
** SYNOPSIS
**	#include "_fpio.h"
**
**	void
**	_fpioinit(void)
**
** DESCRIPTION
**	_fpioinit() initializes I/O data structures for IPW floating-point
**	pixels.  Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_fpiocb
*/

#include "ipw.h"
#include "_fpio.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_fpioinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 *  Initialize array of pointers to FPIO control blocks.
	 */
	_fpiocb = (FPIO_T **) ecalloc(_ipwmaxfd + 1, sizeof(FPIO_T *));
	if (_fpiocb == NULL)
	    error("Insufficient memory for IPW floating-point I/O system");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _fpiocb[i] = NULL;
}
