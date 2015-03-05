#ifndef lint
static char     rcsid[] =
"$Id: mvalloc.c,v 1.2 1992/02/16 19:23:04 thewalt Exp thewalt $";
static char    *copyright = "Copyright (C) 1991, 1992, Chris Thewalt";
#endif

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

#include <stdio.h>
#ifdef __STDC__
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#else  /* not __STDC__ */
#include <varargs.h>
extern void *malloc();
extern void  free();
extern void  exit();
#endif /* not __STDC__ */
#include "mvalloc.h"

typedef double  Align;
typedef void   *VoidPtr;

typedef struct {
    int             magic;
    int             eltype;
    int             dimension;
    long           *size;
    Align           start;
} MVobj;

#define  MV_MAGIC 2099

static MVobj      offset_var;
#define MV_OFFSET ((char*)&offset_var.start-(char*)&offset_var)

static void *
MallocOrDie(size)
long size;
{
    void           *p;
    extern void    *malloc();

    if ((p = malloc(size)) == NULL) {
	fprintf(stderr, "MallocOrDie: insufficient memory\n");
	exit(1);
    }
    return p;
}

#ifdef __STDC__
void *
MValloc(int eltype, int dimension, ...)
#else  /* not __STDC__ */
void *
MValloc(va_alist)
va_dcl
#endif /* not __STDC__ */
{
    va_list         ap;
    MVobj          *mvp;
    char	 ***c3, **c2;
    short        ***s3, **s2;
    int          ***i3, **i2;
    long         ***l3, **l2;
    float        ***f3, **f2;
    double       ***d3, **d2;
    VoidPtr      ***v3, **v2;
    void           *retval = 0;
    long            i, j, *size;
    long            pp_count, p_count, el_count;
    long            base, count;
#ifndef __STDC__
    int             eltype, dimension;
#endif

#ifdef __STDC__
    va_start(ap, dimension);
#else  /* not __STDC__ */
    va_start(ap);
    eltype = va_arg(ap, int);
    dimension = va_arg(ap, int);
#endif /* not __STDC__ */
    el_count = 1;
    if (dimension > 0) {
        size = MallocOrDie((long) dimension * sizeof(long));
	for (i=0; i < dimension; i++) {
	    size[i] = va_arg(ap, long);
	    if (size[i] < 0) {
	        fprintf(stderr, "MValloc: negative size request\n");
		return 0;
	    }
	    el_count *= size[i];
	}
    } else {
	fprintf(stderr, "MValloc: non-positive dimension argument\n");
	return 0;
    }
    va_end(ap);
    if (dimension == 1) {
	p_count = pp_count = 0;
    } else if (dimension == 2) {
	p_count = el_count / size[dimension-1];
	pp_count = 0;
    } else {
	pp_count = 0;
	p_count = 1;
	for (i = 0; i < dimension - 2; i++) {
	    p_count *= size[i];
	    pp_count += p_count;
	}
	p_count = el_count / size[dimension-1];
    }
    base = 0;
    count = 1;
    switch (eltype) {
      case MV_CHAR:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(char **));
	    retval = c3 = (char ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    c3[base+j] = (char **)(c3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    c3[base] = MallocOrDie((long) p_count * sizeof(char *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		c3[base+i] = c3[base+i-1] + size[dimension-2];
	    c3[base][0] = MallocOrDie((long) el_count * sizeof(char));
	    for (i = 1; i < p_count; i++)
	        c3[base][i] = c3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(char *));
	    retval = c2 = (char **)((char *)mvp + MV_OFFSET);
	    c2[0] = MallocOrDie((long) el_count * sizeof(char));
	    for (i = 1; i < p_count; i++)
	        c2[i] = c2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(char));
	    retval = (char *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_SHORT:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(short **));
	    retval = s3 = (short ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    s3[base+j] = (short **)(s3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    s3[base] = MallocOrDie((long) p_count * sizeof(short *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		s3[base+i] = s3[base+i-1] + size[dimension-2];
	    s3[base][0] = MallocOrDie((long) el_count * sizeof(short));
	    for (i = 1; i < p_count; i++)
	        s3[base][i] = s3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(short *));
	    retval = s2 = (short **)((char *)mvp + MV_OFFSET);
	    s2[0] = MallocOrDie((long) el_count * sizeof(short));
	    for (i = 1; i < p_count; i++)
	        s2[i] = s2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(short));
	    retval = (short *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_INT:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(int **));
	    retval = i3 = (int ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    i3[base+j] = (int **)(i3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    i3[base] = MallocOrDie((long) p_count * sizeof(int *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		i3[base+i] = i3[base+i-1] + size[dimension-2];
	    i3[base][0] = MallocOrDie((long) el_count * sizeof(int));
	    for (i = 1; i < p_count; i++)
	        i3[base][i] = i3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(int *));
	    retval = i2 = (int **)((char *)mvp + MV_OFFSET);
	    i2[0] = MallocOrDie((long) el_count * sizeof(int));
	    for (i = 1; i < p_count; i++)
	        i2[i] = i2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(int));
	    retval = (int *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_LONG:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(long **));
	    retval = l3 = (long ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    l3[base+j] = (long **)(l3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    l3[base] = MallocOrDie((long) p_count * sizeof(long *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		l3[base+i] = l3[base+i-1] + size[dimension-2];
	    l3[base][0] = MallocOrDie((long) el_count * sizeof(long));
	    for (i = 1; i < p_count; i++)
	        l3[base][i] = l3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(long *));
	    retval = l2 = (long **)((char *)mvp + MV_OFFSET);
	    l2[0] = MallocOrDie((long) el_count * sizeof(long));
	    for (i = 1; i < p_count; i++)
	        l2[i] = l2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(long));
	    retval = (long *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_FLOAT:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(float **));
	    retval = f3 = (float ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    f3[base+j] = (float **)(f3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    f3[base] = MallocOrDie((long) p_count * sizeof(float *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		f3[base+i] = f3[base+i-1] + size[dimension-2];
	    f3[base][0] = MallocOrDie((long) el_count * sizeof(float));
	    for (i = 1; i < p_count; i++)
	        f3[base][i] = f3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(float *));
	    retval = f2 = (float **)((char *)mvp + MV_OFFSET);
	    f2[0] = MallocOrDie((long) el_count * sizeof(float));
	    for (i = 1; i < p_count; i++)
	        f2[i] = f2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(float));
	    retval = (float *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_DOUBLE:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long) MV_OFFSET + pp_count * sizeof(double **));
	    retval = d3 = (double ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    d3[base+j] = (double **)(d3 + base + count + j * size[i+1]);
	        }
                base += count;
	    }
	    d3[base] = MallocOrDie((long) p_count * sizeof(double *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		d3[base+i] = d3[base+i-1] + size[dimension-2];
	    d3[base][0] = MallocOrDie((long) el_count * sizeof(double));
	    for (i = 1; i < p_count; i++)
	        d3[base][i] = d3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(double *));
	    retval = d2 = (double **)((char *)mvp + MV_OFFSET);
	    d2[0] = MallocOrDie((long) el_count * sizeof(double));
	    for (i = 1; i < p_count; i++)
	        d2[i] = d2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(double));
	    retval = (double *)((char *)mvp + MV_OFFSET);
	}
	break;
      case MV_VOIDP:
	if (dimension > 2 ) {
	    mvp = MallocOrDie((long)MV_OFFSET + pp_count * sizeof(VoidPtr **));
	    retval = v3 = (VoidPtr ***)((char *)mvp + MV_OFFSET);
	    for (i = 0; i < dimension - 3; i++) {
	        count *= size[i];
	        for (j = 0; j < count; j++) {
		    v3[base+j] = (VoidPtr **)(v3 + base + count + j*size[i+1]);
	        }
                base += count;
	    }
	    v3[base] = MallocOrDie((long) p_count * sizeof(VoidPtr *));
	    count *= size[dimension-3];
	    for (i = 1; i < count; i++) 
		v3[base+i] = v3[base+i-1] + size[dimension-2];
	    v3[base][0] = MallocOrDie((long) el_count * sizeof(VoidPtr));
	    for (i = 1; i < p_count; i++)
	        v3[base][i] = v3[base][i-1] + size[dimension-1];
	} else if (dimension == 2) {
	    mvp = MallocOrDie((long) MV_OFFSET + p_count * sizeof(VoidPtr *));
	    retval = v2 = (VoidPtr **)((char *)mvp + MV_OFFSET);
	    v2[0] = MallocOrDie((long) el_count * sizeof(VoidPtr));
	    for (i = 1; i < p_count; i++)
	        v2[i] = v2[i-1] + size[1];
	} else {
	    mvp = MallocOrDie((long) MV_OFFSET + el_count * sizeof(VoidPtr));
	    retval = (VoidPtr *)((char *)mvp + MV_OFFSET);
	}
	break;
      default:
	fprintf(stderr, "MValloc: illegal element type\n");
	free((void *) size);
	return 0;
    }
    if (retval) {
	mvp->magic = MV_MAGIC;
	mvp->eltype = eltype;
	mvp->dimension = dimension;
	mvp->size = size;
    }
    return retval;
}

void
MVfree(mvobj)
void *mvobj;
{
    MVobj          *mvp = 0;
    char	 ***c3, **c2;
    short        ***s3, **s2;
    int          ***i3, **i2;
    long         ***l3, **l2;
    float        ***f3, **f2;
    double       ***d3, **d2;
    VoidPtr      ***v3, **v2;
    long            count, base;
    int             i;

    if (mvobj) {
	mvp = (MVobj *) ((char *)mvobj - MV_OFFSET);
	if (mvp->magic != MV_MAGIC) {
            fprintf(stderr, "MVfree: argument isn't an MVobject\n");
	    return;
	}
	mvp->magic = 0;  /* in case we see this object again */
    } else  {
        fprintf(stderr, "MVfree: NULL argument\n");
	return;
    }
    base = 0;
    count = 1;
    switch (mvp->eltype) {
      case MV_CHAR:
	if (mvp->dimension > 2 ) {
	    c3 = (char ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) c3[base][0]);
	    free((void *) c3[base]);
	} else if (mvp->dimension == 2) {
	    c2 = (char **) mvobj;
	    free((void *) c2[0]);
	}
	break;
      case MV_SHORT:
	if (mvp->dimension > 2 ) {
	    s3 = (short ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) s3[base][0]);
	    free((void *) s3[base]);
	} else if (mvp->dimension == 2) {
	    s2 = (short **) mvobj;
	    free((void *) s2[0]);
	}
	break;
      case MV_INT:
	if (mvp->dimension > 2 ) {
	    i3 = (int ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) i3[base][0]);
	    free((void *) i3[base]);
	} else if (mvp->dimension == 2) {
	    i2 = (int **) mvobj;
	    free((void *) i2[0]);
	}
	break;
      case MV_LONG:
	if (mvp->dimension > 2 ) {
	    l3 = (long ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) l3[base][0]);
	    free((void *) l3[base]);
	} else if (mvp->dimension == 2) {
	    l2 = (long **) mvobj;
	    free((void *) l2[0]);
	}
	break;
      case MV_FLOAT:
	if (mvp->dimension > 2 ) {
	    f3 = (float ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) f3[base][0]);
	    free((void *) f3[base]);
	} else if (mvp->dimension == 2) {
	    f2 = (float **) mvobj;
	    free((void *) f2[0]);
	}
	break;
      case MV_DOUBLE:
	if (mvp->dimension > 2 ) {
	    d3 = (double ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) d3[base][0]);
	    free((void *) d3[base]);
	} else if (mvp->dimension == 2) {
	    d2 = (double **) mvobj;
	    free((void *) d2[0]);
	}
	break;
      case MV_VOIDP:
	if (mvp->dimension > 2 ) {
	    v3 = (VoidPtr ***) mvobj;
	    for (i = 0; i < mvp->dimension - 3; i++) {
	        count *= mvp->size[i];
                base += count;
	    }
	    free((void *) v3[base][0]);
	    free((void *) v3[base]);
	} else if (mvp->dimension == 2) {
	    v2 = (VoidPtr **) mvobj;
	    free((void *) v2[0]);
	}
	break;
      default:
	fprintf(stderr, "MVfree: illegal element type\n");
	break;
    }
    free((void *) mvp->size);
    free((void *) mvp);
}

int
MVdimension(mvobj)
void *mvobj;
{
    MVobj         *mvp;
    int           dimension = -1;

    if (mvobj) {
	mvp = (MVobj *) ((char *)mvobj - MV_OFFSET);
	if (mvp->magic == MV_MAGIC)
	    dimension = mvp->dimension;
	else 
            fprintf(stderr, "MVdimension: argument isn't an MVobject\n");
    } else
        fprintf(stderr, "MVdimension: NULL argument\n");
    return dimension;
}

long
MVsize(mvobj, dimension)
void *mvobj;
int   dimension;
{
    MVobj         *mvp;
    long           size = -1;

    if (mvobj) {
	mvp = (MVobj *) ((char *)mvobj - MV_OFFSET);
	if (mvp->magic == MV_MAGIC) {
	    if (mvp->dimension >= dimension)
	        size = mvp->size[dimension];
	    else
                fprintf(stderr,"MVsize: bad dimension for specified object\n");
	} else
            fprintf(stderr, "MVsize: first argument isn't an MVobject\n");
    } else
        fprintf(stderr, "MVsize: first argument is NULL\n");
    return size;
}

int
MVtype(mvobj)
void *mvobj;
{
    MVobj         *mvp;
    int           type = -1;

    if (mvobj) {
	mvp = (MVobj *) ((char *)mvobj - MV_OFFSET);
	if (mvp->magic == MV_MAGIC)
	    type = mvp->eltype;
	else 
            fprintf(stderr, "MVtype: argument isn't an MVobject\n");
    } else
        fprintf(stderr, "MVtype: NULL argument\n");
    return type;
}
