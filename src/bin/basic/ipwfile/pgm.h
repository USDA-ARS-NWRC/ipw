#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "ipwfile" program
 */

typedef struct {
	int             i_fd;		/* input image file descriptor	 */
	int		flags;
	bool_t		machread;
} PARM_T;

#define DOLINES		BIT(1)
#define DOSAMPS		BIT(2)
#define DOBANDS		BIT(3)
#define DOBYTES		BIT(4)
#define DOFILE		BIT(5)
#define MACHINEREAD	BIT(6)
#define ISSET(bit)	(((parm.flags)&bit) == bit)
#define SETBIT(bit)	parm.flags = parm.flags | bit


extern PARM_T   parm;

extern void     headers(void);

#endif
