#ifndef MVALLOC_H
#define MVALLOC_H

/*
 * Copyright (C) 1991, 1992 by Chris Thewalt (thewalt@ce.berkeley.edu)
 *
 * Permission to use, copy, modify, and distribute this software 
 * for any purpose and without fee is hereby granted, provided
 * that the above copyright notices appear in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation.  This software is provided "as is" without express or
 * implied warranty.
 */


#define MV_CHAR     123
#define MV_SHORT    124
#define MV_INT      125
#define MV_LONG     126
#define MV_FLOAT    127
#define MV_DOUBLE   128
#define MV_VOIDP    129

#ifdef __STDC__
void           *MValloc(int eltype, int dimension, ...);
void            MVfree(void *mvobj);
int             MVdimension(void *mvobj);
long            MVsize(void *mvobj, int dimension);
int             MVtype(void *mvobj);
#else  /* not __STDC__ */
void           *MValloc();
void            MVfree();
int             MVdimension();
long            MVsize();
int             MVtype();
#endif /* not __STDC__ */

#endif /* not MVALLOC_H */
