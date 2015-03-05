#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "despike" program
 */

typedef struct {
	int             im1;		/* input image file descriptor	 */
	int             maskfile;	/* mask image file descriptor	 */
	int             outfile;	/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	int		nlines;
	int		nsamps;
	double		lowpass;	/* values below this untouched	 */
	double		threshmult;	/* threshold multiplier		 */
} PARM_T;

PARM_T   parm;

extern void headers(void);
extern void despike(void);

#endif
