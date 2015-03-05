#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	DFLT_RANK	4		/* dither matrix default rank	 */

#define	MAX_RANK	16
#define	MIN_RANK	2

#define	BLACK		0
#define	WHITE		1

extern int	dm_val(int rank, int row, int col);
extern void	dither(int i_fd, int rank, int o_fd);
extern void	dithimg(int i_fd, int nlines, int nsamps,
			int nbands, pixel_t ***dm, int rank, int o_fd);
extern pixel_t	***mk_dm(int rank, int nbands, BIH_T **bihpp);

#endif
