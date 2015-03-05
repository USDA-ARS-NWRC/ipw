#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "mstats" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             nlines;		/* #lines / image		 */
	int             nsamps;		/* #samples / line	 	 */
	int             i_nbands;	/* #bands / input sample	 */
	fpixel_t       *i_buf;		/* -> input line buffer		 */
	double        **sum_x;		/* sum[class][band]		 */
	double       ***sum_xy;		/* sum[class][band][band]	 */
	pixel_t        *c_buf;		/* -> class line buffer		 */
	int            *c_npixels;	/* #pixels[class]		 */
	int             c_fd;		/* class image file descriptor	 */
	int             c_nclasses;	/* #classes			 */
} PARM_T;

extern PARM_T   parm;

extern void     accum(void);
extern void     headers(void);
extern void     init(void);
extern void     mcov(void);
extern void     mstats(void);

#endif
