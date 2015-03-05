#ifndef	_BIH_H
#define	_BIH_H

#include "bih.h"

/*
 *  Global variables that are private to this module.
 */

extern BIH_T *** _bih;
extern bool_t *  _no_hist;

/* ------------------------------------------------------------------------ */

/*
 *  Functions that are private to this module.
 */

extern bool_t	 _bihcheck(BIH_T **hpp);
extern void	 _bihfree(int fd);
extern void	 _bihinit(void);

#endif  /* _BIH_H */
