#include "IPWmacros.h"

#include "lqh.h"

/*
 * header file for "requant" program
 */

extern void   requant(int fdi, bool_t got_lqh, int fdh, bool_t got_lohi,
			fpixel_t *newmm, bool_t got_bits, int newbits,
			bool_t got_verb, int fdo);
extern int    lqhremap(LQH_T *lqhp, int oldbits, int newbits);
extern int    lqhprint(LQH_T *lqhp);
