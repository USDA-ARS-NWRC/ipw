/*
** NAME
**	crh -- get CR header associated with file descriptor
**
** SYNOPSIS
**	#include "crh.h"
**
**	CRH_T **
**	crh(
**		int		fd)	|* image file descriptor	*|
**
** DESCRIPTION
**
** RETURN VALUE
**	pointer to the CR header array associated with file descriptor fd; or
**	NULL if there are no CR headers associated with file descriptor fd.
**
** GLOBALS ACCESSED
**	_crh
*/

#include "ipw.h"
#include "_crh.h"

CRH_T **
crh(
	int		fd)		/* image file descriptor	*/
{
	ASSERT_OK_FD(fd);
	return (_crh[fd]);
}
