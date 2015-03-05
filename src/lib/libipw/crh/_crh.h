#ifndef	_CRH_H
#define	_CRH_H

#include "crh.h"

/*
 *  Global variables that are private to this module.
 */

extern CRH_T *** _crh;

/* ------------------------------------------------------------------------ */

/*
 *  Functions that are private to this module.
 */

extern int	 _crharrays(CRH_T *crh);
extern bool_t	 _crhcheck(CRH_T **crhpp, int nbands);
extern void	 _crhinit(void);

#endif  /* _CRH_H */
