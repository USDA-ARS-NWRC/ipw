#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define	NBITS_ROUND	16

#define START_BIT	BIT(CHAR_BIT - 1)
#define NEXT_BIT(i)	( (i) >>= 1 )

#define	is_set(i)	( ((i) & 1) == 0 )

extern void     bitmap(REG_1 char *in, REG_3 char *out, REG_5 int nsamps); 
extern int      sunhdr(int nlines, int nsamps, int nbits, int o_fd);
extern void     sunras(int i_fd, int o_fdi);

#endif
