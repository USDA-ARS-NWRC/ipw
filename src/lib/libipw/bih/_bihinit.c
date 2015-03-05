/*
** NAME
**	_bihinit -- initialize IPW data structures for LQ headers
**
** SYNOPSIS
**	#include "_bih.h"
**
**	void
**	_bihinit(void)
**
** DESCRIPTION
**	_bihinit() initializes I/O data structures for IPW basic image headers.
**	Must be called after the _uioinit() routine.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES MODIFIED
**	_bih
*/

#include "ipw.h"
#include "_bih.h"
#include "../uio/_uio.h"	/* for _ipwmaxfd */

void
_bihinit(void)
{
	int	     i;			/* loop index			 */

	/*
	 * initialize o_byteorder to hostorder() - may be changed after
	 * ipwenter if a routine plans to bypass the pixio byte swapping
	 * layer (e.g., transpose, flip, and window)
	 */
	o_byteorder = hostorder();

	/*
	 *  Initialize array of pointers to basic image headers.
	 */
	_bih = (BIH_T ***) ecalloc(_ipwmaxfd + 1, sizeof(BIH_T **));
	if (_bih == NULL)
	    error("Insufficient memory for IPW basic image headers");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _bih[i] = NULL;

	/*
	 *  Initialize array of "no-history" flags
	 */
	_no_hist = (bool_t *) ecalloc(_ipwmaxfd + 1, sizeof(bool_t));
	if (_no_hist == NULL)
	    error("Insufficient memory for IPW basic image headers");
	for (i = 0; i <= _ipwmaxfd; i++)
	    _no_hist[i] = FALSE;
}
