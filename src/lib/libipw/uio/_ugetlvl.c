/*
** NAME
**	ugetlevel -- get maximum I/O level used in UIO control block
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int
**	_ugetlvl(
**		int	fd)		|* file descriptor	*|
**
** DESCRIPTION
**	ugetlevel gets the maximum I/O level used in the UIO control block.
**	This is used to check which version of close should be called.
**
** RETURN VALUE
**	current level of the open IPW file
**
** GLOBALS ACCESSED
**	_uiocb
**
*/

#include "ipw.h"
#include "_uio.h"

int
_ugetlvl(
	int	fd)		/* file descriptor	*/
{
	ASSERT_OK_FD(fd);

	return(_uiocb[fd].level);
}
