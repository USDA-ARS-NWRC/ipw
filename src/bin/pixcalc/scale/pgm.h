#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#define OP1	'+'	/* add a constant (x + v) */
#define OP2	'-'	/* subtract a constant (x - v) */
#define OP3	'*'	/* multiply by a constant (x * v) */
#define OP4	'/'	/* divide by a constant (x / v) */
#define OP5	'^'	/* raise to the power of a constant (pow(x,v)) */
#define OP6	'q'	/* take the square root (sqrt(x)) */
#define OP7	'l'	/* take the natural log (ln(x), in c: log(x)) */
#define OP8	'e'	/* e**x (exp(x)) */
#define OP9	'g'	/* take the log, base 10 (log10(x)) */
#define OP10	'x'	/* 10**x (pow(10,x)) */

/*
 * header file for "scale" program
 */

typedef struct {
	char           *tmpnam;		/* name of scratch file		 */
	int             i_fd;		/* input image file descriptor	 */
	int             m_fd;		/* mask image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	int             nbits;		/* # output bits per pixel	 */
	int             nrbands;	/* # bands to reciprocate	 */
	int            *rband;		/* -> reciprocal band numbers	 */
	fpixel_t        mmval[2];	/* min, max in output vector	 */
	fpixel_t	no_data;	/* no data value 		 */
	bool_t		got_no_data;	/* flag if no data value set	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers(bool_t already);
extern void     scale(char aop, float val);
extern void     output(void);
extern void	fast_scale(char aop, float val);            

#endif
