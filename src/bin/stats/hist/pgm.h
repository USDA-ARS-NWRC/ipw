#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#if (SIZEOF_INT == 4)
typedef int	int32_t;
#elif (SIZEOF_LONG == 4)
typedef long	int32_t;
#elif (SIZEOF_SHORT == 4)
typedef short	int32_t;
#else
error "None of int, long, short is 4 bytes wide"
#endif

typedef int32_t hist_t;

extern void	hist(int i_fd, int m_fd, int h_fd); 
extern void	histx(REG_1 pixel_t *i_buf, REG_3 pixel_t *m_buf,
		      REG_6 int nsamps, REG_5 int nbands, REG_2 hist_t **histo); 

#endif
