/*
** NAME
**	_uioexit -- close all IPW files
**
** SYNOPSIS
**	#include "_uio.h"
**
**	void
**	_uioexit(void)
**
** DESCRIPTION
**	_uioexit closes all open IPW files.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES READ
**	_ipwfds
**
** GLOBAL VARIABLES MODIFIED
**	_ipwnumfds
*/

#include "ipw.h"

#include "_uio.h"

void
_uioexit(void)
{
	int	      *fdp;		/* -> IPW file descriptor	 */
	int            level;		/* IO level of current file	 */

	/*
	 *  Close all open IPW files.
	 */
	fdp = _ipwfds;
	while (_ipwnumfds > 0) {
	    level = _ugetlvl(*fdp);
	    switch (level) {
		case FTYPE_UIO :
		    if (uclose(*fdp) == ERROR)
			error("Problem closing file \"%s\"", ufilename(*fdp));
		    break;

		case FTYPE_PIXIO :
		    if (pxclose(*fdp) == ERROR)
			error("Problem closing file \"%s\"", ufilename(*fdp));
		    break;

		case FTYPE_FPIO :
		    if (fpclose(*fdp) == ERROR)
			error("Problem closing file \"%s\"", ufilename(*fdp));
		    break;

		default :
		    bug("Unknown level");
	    }  /* switch */
	}  /* for */
}
