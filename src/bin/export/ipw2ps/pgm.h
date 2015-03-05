#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	PAGE_HEIGHT	11		/* output page height		 */
#define	PAGE_WIDTH	8.5		/* output page width		 */

#define	IMG_HEIGHT	9.5		/* max. height of output image	 */
#define	IMG_WIDTH	7.0		/* max. width of output image	 */

#define	XTRANS_FUDGE	(1.0 / 16.0)	/* LaserWriter X bias		 */

#define	HI_NYBBLE(i)	( ((i) >> 4) & 0xF )
#define	LO_NYBBLE(i)	( (i) & 0xF )

#define	ITOP(in)	( (int)((in) * 72.0) )

extern void     pspic(int fdi, double h_page, double w_page,
		      double h_img, double w_img, bool_t rotate);
extern void     pspicx(int fdi, int nlines, int nsamps, int nbits, double h_page,
		       double w_page, double h_img, double w_img, bool_t rotate);
#endif
