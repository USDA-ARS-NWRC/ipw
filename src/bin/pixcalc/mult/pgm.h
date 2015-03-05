#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"
#include "fpio.h"
#include "pixio.h"

/*
 * header file for "mult" program
 */

typedef struct {
	char           *tmpnam;		/* name of scratch file		 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	int             nrbands;	/* # bands to reciprocate	 */
	int            *rband;		/* -> reciprocal band numbers	 */
	fpixel_t        mmval[2];	/* min, max in output vector	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(bool_t already);
extern void     mult(void);
extern void     output(void);

#endif
