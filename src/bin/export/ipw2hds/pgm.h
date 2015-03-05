#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "hdspic" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             i_nlines;	/* # input image lines		 */
	int             i_nsamps;	/* # samples / input image line	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(void);
extern void     hdspic(void);

#endif
