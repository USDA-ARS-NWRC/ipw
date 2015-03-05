#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

typedef struct {
	int		force;		/* flag for force coincident	 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             skip_lines;	/* line shrink factor		 */
	int             dup_lines;	/* line zoom factor		 */
	int             skip_samps;	/* sample shrink factor		 */
	int             dup_samps;	/* sample zoom factor		 */
	int             nbands;		/* # image bands		 */
	int             i_nlines;	/* # input lines		 */
	int             i_nsamps;	/* # samples / input line	 */
	int             o_nlines;	/* # output lines		 */
	int             o_nsamps;	/* # samples / output line	 */
	pixel_t        *buf;		/* -> image line buffer		 */
} PARM_T;

extern PARM_T   parm;

extern void     fixhdrs(void);
extern void     headers(void);
extern void     replicate(void);
extern void     subsamp(void);
extern void     zoom(void);

#endif
