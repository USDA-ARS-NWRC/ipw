#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "deltaT" program
 */

typedef struct {
	char           *tmpnam;		/* name of scratch file		 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	fpixel_t        mmval[2];	/* min, max in output vector	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(bool_t already);
static void     head1(void);
static void     head2(void);
extern void     deltaT(void);
extern void     output(void);

#endif
