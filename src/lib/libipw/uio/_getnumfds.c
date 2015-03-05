/*
** NAME
**	_getnumfds -- get maximum number of file descriptors
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int
**	_getnumfds(void)
**
** DESCRIPTION
**	returns the maximum number of files IPW will allow to be opened.
**	It attempts to determine the O/S soft-limit (dynamically settable)
**	value with a call to sysconf().  If sysconf() fails, the function
**	calls error().
**
** RETURN VALUE
**	maximum number of file descriptors
**
** HISTORY
*/


#include <unistd.h>

#include "ipw.h"

int
_getnumfds(void)
{
	long openmax;

	openmax = sysconf(_SC_OPEN_MAX);

	if (openmax == -1) {
		syserr();
		error("Cannot get system configuration info for _SC_OPEN_MAX");
	}

	return( (int) openmax );
}
