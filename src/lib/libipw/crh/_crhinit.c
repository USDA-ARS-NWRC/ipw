/*
** NAME
**	_crhinit -- initialize IPW data structures for class range headers
**
** SYNOPSIS
**	#include "_crh.h"
**
**	void
**	_crhinit(void)
**
** DESCRIPTION
**	_crhinit() initializes I/O data structures for IPW class range headers.
**	Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_crh
*/

#include "ipw.h"
#include "_crh.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_crhinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 *  Initialize array of pointers to class range headers.
	 */
	_crh = (CRH_T ***) ecalloc(_ipwmaxfd + 1, sizeof(CRH_T **));
	if (_crh == NULL)
	    error("Insufficient memory for IPW class range headers");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _crh[i] = NULL;
}
