#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "lqh.h"

#define	IBANDS 2

/*
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
*/

extern void	fillbands(int fdi, int fdo, int outbands);
extern void	headers(int fdi, int fdo, int outbands);
extern LQH_T  **newlqh(int fdo, LQH_T **i_lqh);

#endif
