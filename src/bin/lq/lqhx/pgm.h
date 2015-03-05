
#include "IPWmacros.h"

typedef struct {
	int             h_fd;		/* input header file descriptor	 */
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
} PARM_T;

extern PARM_T   parm;

extern void     headers();
extern void     lqhx();
