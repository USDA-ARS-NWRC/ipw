/*
** NAME
**	usetlevel -- set maximum I/O level used in UIO control block
**
** SYNOPSIS
**	#include "_uio.h"
**
**	void
**	_usetlvl(
**		int	fd,		|* file descriptor		*|
**	  	int	level)		|* new level for file desc	*|
**
** DESCRIPTION
**	usetlevel sets the maximum I/O level used in the UIO control block.
**	This is used to check which version of close should be called.
**
**	The level will be strictly increasing unless level is set to a
**	negative value.
**
** RETURN VALUE
**
** GLOBALS ACCESSED
**	_uiocb
*/

#include "ipw.h"
#include "_uio.h"

void
_usetlvl(
	int	fd,		/* file descriptor		*/
  	int	level)		/* new level for file desc	*/
{
	ASSERT_OK_FD(fd);

	if (level < 0) {
		assert (-level > FTYPE_MIN);
		assert (-level < FTYPE_MAX);
		_uiocb[fd].level = -level;
	} else {
		assert (level > FTYPE_MIN);
		assert (level < FTYPE_MAX);
		_uiocb[fd].level = MAX( _uiocb[fd].level , level );
	}
}
