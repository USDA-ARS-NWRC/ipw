
#ifndef	PGM_H
#define	PGM_H

#include "fpio.h"
#include "pixio.h"

/*
 * header file for "mult" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers();
extern void     norm();
extern void     output();

#endif
