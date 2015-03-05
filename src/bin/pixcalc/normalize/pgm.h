#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "normalize" program
 */

typedef struct {
	char           *tmpnam;		/* name of scratch file		 */
	int             i_fd;		/* input image file descriptor	 */
	int             m_fd;		/* mask image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	int             nbands;	        /* # bands 			 */
	int		nlines;
 	int	 	nsamps;
	fpixel_t        mmval[2];	/* min, max in output vector	 */
	fpixel_t	no_data;	/* no data value 		 */
	bool_t		got_no_data;	/* flag if no data value set	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(void);
extern void     normalize(void);
extern void     output(void);

#endif
