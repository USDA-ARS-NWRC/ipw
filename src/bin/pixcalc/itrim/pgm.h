#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "itrim" program
 */

typedef struct {
	char           *tmpnam;		/* name of scratch file		 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	int             ibands;		/* # input bands 		 */
	int             obands;		/* # output bands 		 */
	fpixel_t      **mmval;		/* min, max in output vector	 */
	bool_t		bin;	 	/* binary output flag 		 */
	bool_t		cap;	 	/* cap output flag		 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(bool_t already);
extern void	head1(void);
extern void	head2(void);
extern void     scale(float min, float max, float nodatamin, float nodatamax);
extern void     output(void);

#endif
