/*
** NAME
** 	nothing -- return 0
** 
** SYNOPSIS
**	int
**	nothing (ibuf, line, samp, nlines, nsamps)
**	fpixel_t **ibuf;
**	float line, samp;
**	int nlines, nsamps;
** 
** DESCRIPTION
**	nothing is here for timing purposes only!
** 
** RETURN VALUE
**	0
**
*/

#include "ipw.h"
#include "pgm.h"
#include "gctp.h"

int
nothing(
	pixel_t       **ibuf,		/* input buffer			 */
	float		line,		/* line index (decimal)		 */
	float		samp,		/* sample index (decimal)	 */
	int		nlines,		/* # lines in buffer		 */
	int		nsamps)		/* # samples in buffer		 */
{
	return (0);
}
