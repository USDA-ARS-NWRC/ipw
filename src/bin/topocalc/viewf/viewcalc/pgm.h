#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#include "ipw.h"
#include "fpio.h"
#include "lqh.h"

/*
 * header file for "viewcalc" program
 */

typedef struct {
	int             i_fds;		/* input S/A file desc	 */
	int             i_fdh;		/* input horizon file desc */
	int             o_fd;		/* output image file desc */
	float          *cstbl;		/* cosines of slopes	 */
	float          *hazm;		/* horizon azimuths	 */
	float         **cosdtbl;	/* cos(phi - A)		 */
	float         **hdtbl;		/* H - sinH cos H	 */
	float         **sh2tbl;		/* sin^2 (H)		 */
	fpixel_t       *obuf;		/* output buffer	 */
	fpixel_t       *sstbl;		/* sines of slopes	 */
	fpixel_t      **chtbl;		/* cosines of horizons	 */
	pixel_t        *hbuf;		/* input horizon buff	 */
	pixel_t        *sbuf;		/* input slope/azm buff */
}               PARM_T;

extern PARM_T   parm;

extern void     buffers(void);
extern void     headers(void);
extern LQH_T  **newlqh(int fdi, int fdo);
extern void     trigtbl(void);
extern void     viewf(void);

#endif
