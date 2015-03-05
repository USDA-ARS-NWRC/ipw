#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "ipw2xim" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / line		 */
	int             nbands;		/* # bands / sample		 */
	int             nlevels;	/* # levels / pixel		 */
} PARM_T;

#define	OUT_NLEVELS	256

extern PARM_T   parm;

extern void     headers(void);
extern void     ipw2xim(void);
extern void     ximhdr(void);

#endif
