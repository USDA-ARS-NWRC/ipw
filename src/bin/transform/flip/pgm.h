#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

#include "ipw.h"

#include "geoh.h"
#include "orh.h"
#include "winh.h"

/*
 * header file for "flip" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
	bool_t          lines;		/* ? flip lines			 */
	bool_t          samps;		/* ? flip samples		 */
}               PARM_T;

extern PARM_T   parm;

extern GEOH_T **fixgeoh(GEOH_T **i_geoh);
extern ORH_T  **fixorh(ORH_T **i_orh);
extern WINH_T **fixwinh(WINH_T **i_winh);
extern void     flip(void);
extern void     flip1d(int nl, int ns, int nb, addr_t buf);
extern void     flip2d(int nl, int ns, int nb, addr_t *buf);
extern void     headers(void);
extern void     reverse(addr_t a, int nelem, int nb);
extern void     revgen(register addr_t a, int nelem, register int nb);
extern void     revchar(register char *vec, register int n);
extern void     revlong(register long *vec, register int n);
extern void     revshort(register short *vec, register int n);

#endif
