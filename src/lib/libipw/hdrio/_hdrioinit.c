/*
** NAME
**	_hdrioinit -- initialize IPW image-header I/O system
**
** SYNOPSIS
**	#include "_hdrio.h"
**
**	void
**	_hdrioinit(void)
**
** DESCRIPTION
**	_hdrioinit() initializes I/O data structures for IPW image headers.
**	Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_hdriocb
*/

#include <strings.h>

#include "ipw.h"
#include "_hdrio.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_hdrioinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 *  Initialize HDRIO control blocks.
	 */
	_hdriocb = (HDRIO_T *) ecalloc(_ipwmaxfd + 1, sizeof(HDRIO_T));
	if (_hdriocb == NULL)
	    error("Insufficient memory for IPW image-header I/O system");
	for (i = 0; i <= _ipwmaxfd; i++)
	    bzero((void *) (_hdriocb + i), (size_t) sizeof(HDRIO_T));
}
