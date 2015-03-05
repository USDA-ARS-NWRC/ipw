#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define MAXLINE 80

/*
 * header file for "ipw2pbm" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nlines;		/* # image lines		 */
	int             nsamps;		/* # samples / line		 */
	int             nbands;		/* # bands / sample (1 or 3)	 */
	int             nbits;		/* # bits / pixel (all bands same) */
} PARM_T;

extern PARM_T   parm;

#define	PGMCOOKIE	"P5"
#define	PPMCOOKIE	"P6"

extern void     pnm_header(void);
extern void     pnm2ipw(void);

#endif
