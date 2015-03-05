/*
** NAME
** 	histx - accumulate histogram for image line
** 
** SYNOPSIS
**	#include "classeq.h"
**
**	histx (i_buf, m_buf, nsamps, histo)
**	pixel_t i_buf, m_buf;
**	int nsamps;
**	hist_t *histo;
** 
** DESCRIPTION
** 	histx accumulates the histogram for the given image and mask lines.
*/

#include "ipw.h"

#include "pgm.h"

void
histx (
	REG_1 pixel_t  *i_buf,		/* -> image line		 */
	REG_3 pixel_t  *m_buf,		/* -> mask line			 */
	REG_6 int       nsamps,		/* # samples / line		 */
	REG_2 hist_t   *histo)		/* -> histogram array		 */
{
	do {
		if (m_buf == NULL || *m_buf++ != 0) {
			++histo[*i_buf++];
		}
	} while (--nsamps > 0);
}
