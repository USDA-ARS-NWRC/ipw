/*
** NAME
**	_lqhinit -- initialize IPW data structures for LQ headers
**
** SYNOPSIS
**	#include "_lqh.h"
**
**	void
**	_lqhinit(void)
**
** DESCRIPTION
**	_lqhinit() initializes I/O data structures for IPW LQ headers.
**	Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_lqh
*/

#include "ipw.h"
#include "_lqh.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_lqhinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 *  Initialize array of pointers to linear-quantization headers.
	 */
	_lqh = (LQH_T ***) ecalloc(_ipwmaxfd + 1, sizeof(LQH_T **));
	if (_lqh == NULL)
	    error("Insufficient memory for IPW LQ headers");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _lqh[i] = NULL;
}
