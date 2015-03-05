/*
** NAME
**	_uioinit -- initialize IPW I/O system
**
** SYNOPSIS
**	#include "_uio.h"
**
**	void
**	_uioinit(void)
**
** DESCRIPTION
**	_uioinit initializes IPW I/O data structures.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_ipwfds
**	_ipwmaxfd
**	_ipwnumfds
**	_uiocb
*/

#include <strings.h>

#include "ipw.h"
#include "_uio.h"

void
_uioinit(void)
{
	int          numfds;		/* maximum # of open file descs	 */
	int	     i;			/* loop index			 */

	/*
	 *  Determine the maximum # of open files allowed.
	 */
	numfds = _getnumfds();
	_ipwmaxfd = numfds - 1;

	/*
	 *  Initialize list of open file descriptors.
	 */
	_ipwnumfds = 0;
	_ipwfds = (int *) ecalloc(numfds, sizeof(int));
	if (_ipwfds == NULL)
	    error("Insufficient memory for IPW I/O system");
	for (i = 0; i < numfds; i++)
	    _ipwfds[i] = NO_FD;

	/*
	 *  Initialize UIO control blocks.
	 */
	_uiocb = (UIO_T *) ecalloc(numfds, sizeof(UIO_T));
	if (_uiocb == NULL)
	    error("Insufficient memory for IPW I/O system");
	for (i = 0; i < numfds; i++)
	    bzero((void *) (_uiocb + i), (size_t) sizeof(UIO_T));
}
