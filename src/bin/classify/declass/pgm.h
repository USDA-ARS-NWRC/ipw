#ifndef	PGM_H
#define	PGM_H

#define NO_DATA 0

#include "IPWmacros.h"

#include "lqh.h"
#include "crh.h"

extern void	declass (int fdi, int fdo);
extern void	headers (int fdi, int fdh, int *ibits, int ibits_nbands,
			 int fdo);
extern LQH_T ** newlqh (int nbands, int *nbits, CRH_T **crhpp, int fdo);

#endif
