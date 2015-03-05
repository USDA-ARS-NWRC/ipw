/*
** NAME
**	allocnd --- allocate a dynamic, dope-vectored array
**
** SYNOPSIS
**	#include "ipw.h"
**
**	addr_t allocnd(int elsize, int ndim, ...)
**
** DESCRIPTION
**	Allocnd dynamically allocates an ndim-dimensional array, where
**	sizeof(each element) is elsize.  The particular dimensions are given
**	by the dim1, ..., dimn arguments.  The resulting address should be
**	cast into a pointer to the proper type of object.
**
**	The dynamically allocated array is indexed via dope vectors (i.e., all
**	but the rightmost dimension are implemented by arrays of pointers).
**
**	All space allocated by allocnd is initialized to 0.
**
**	This routine depends on the sizeof operator including all padding to
**	the correct boundary, and since ecalloc() ensures that the pointer
**	returned falls on a correct boundary, everything should work out.
**	The one possible problem that I could think of was if character
**	pointers are not the same as other types of pointers (true on some
**	machines).  If this is the case, you're on your own.  Let me know
**	how you fixed this to work in the general case on your machine.
**
**	Because of the extra space required for the dope vectors, arrays
**	allocated by allocnd() will consume more memory than
**	equivalently-dimensioned arrays allocated statically.
**
**	The code for allocnd() has been copied, with minor modifications, from
**	the function dope(), which was posted to USENET with the following
**	copyright notice:
**
**		(c) Copyright 1985 by Jeff Lee and Georgia Institute of
**		Technology
**
**		Explicit permission is given to use, distribute, copy, and do
**		strange things with a fork to this software, but you may not
**		sell it because Georgia Tech's name is on it and I believe in
**		free software.
**
**		Jeff Lee
**		School of ICS, Georgia Institute of Technology, Atlanta
**		gatech!jeff
**
** RETURN VALUE
**	A pointer to the array is returned, or NULL in case of error.
**
** ERRORS
**	size overflow during allocation
**
** EXAMPLES
**	Use allocnd() as a general-purpose multidimensional array allocator.
**	For example:
**
**		double	a[3][4];
**		double	**b;
**
**		b = (double **)allocnd(sizeof(double), 2, 3, 4);
**
**	References of the form b[i][j] will behave equivalently to a[i][j].
**
** BUGS
*/

#include "ipw.h"

#ifdef STANDARD_C

#include <stdarg.h>

addr_t
allocnd(int elsize, int ndim, ...)
{
	va_list         ap;		/* -> variable argument list	 */
	addr_t          array;		/* -> base of allocated memory	 */
	int             bsize;		/* total # bytes allocated	 */
	REG_6 int       dim;		/* current array dimension	 */
	REG_3 int       i;		/* loop counter			 */
	int             size;		/* current dimension product	 */

 /*
  * collect and validate the arguments; calculate total # bytes we need to
  * allocate
  */
	assert(elsize > 0);
	assert(ndim > 0);

	va_start(ap, ndim);


	bsize = 0;
	size = 1;

	for (i = ndim; i > 0; i--) {
		dim = va_arg(ap, int);
		assert(dim > 0);

		size *= dim;
		bsize += size * (i == 1 ? elsize : sizeof(addr_t));
	}

	va_end(ap);

	if (bsize <= 0) {
		usrerr("size overflow during allocation");
		return (NULL);
	}
 /*
  * allocate one big hunk of memory for data + dope vectors
  */
	array = ecalloc(bsize, sizeof(char));
	if (array == NULL) {
		return (NULL);
	}

 /*
  * if 1-D array then all done
  */
	if (ndim < 2) {
		return (array);
	}
 /*
  * initialize the dope vectors
  */
	va_start(ap, ndim);

	dim = va_arg(ap, int);
	bsize = 0;
	size = 1;

	i = ndim - 1;
	do {
		REG_1 addr_t   *dope_p;	/* -> current dope vector	 */
		REG_5 int       j;	/* loop counter			 */
		REG_2 addr_t    next;	/* -> next subarray		 */

 /* NOSTRICT */
		dope_p = (addr_t *) ( (char *) array + bsize);

		size *= dim;
		bsize += size * sizeof(addr_t);
		next = (char *) array + bsize;
 /* NOSTRICT */
		dim = va_arg(ap, int);

		j = size;
		do {
			*dope_p++ = next;
			next = (char *) next +
			       dim * (i == 1 ? elsize : sizeof(addr_t));
		} while (--j > 0);
	} while (--i > 0);

	va_end(ap);
 /*
  * all done
  */
	return (array);
}


#else	/* non ANSI */

#include <varargs.h>

/* VARARGS */
addr_t
allocnd(va_alist)
va_dcl
{
	va_list         ap;		/* -> variable argument list	 */
	addr_t          array;		/* -> base of allocated memory	 */
	int             bsize;		/* total # bytes allocated	 */
	REG_6 int       dim;		/* current array dimension	 */
	REG_4 int       elsize;		/* sizeof(array element)	 */
	REG_3 int       i;		/* loop counter			 */
	int             ndim;		/* # array dimensions		 */
	int             size;		/* current dimension product	 */

 /*
  * collect and validate the arguments; calculate total # bytes we need to
  * allocate
  */
 /* NOSTRICT */
	va_start(ap);
 /* NOSTRICT */
	elsize = va_arg(ap, int);
	assert(elsize > 0);
 /* NOSTRICT */
	ndim = va_arg(ap, int);
	assert(ndim > 0);

	bsize = 0;
	size = 1;

	i = ndim;
	do {
 /* NOSTRICT */
		dim = va_arg(ap, int);
		assert(dim > 0);

		size *= dim;
		bsize += size * (i == 1 ? elsize : sizeof(addr_t));
	} while (--i > 0);

	va_end(ap);

	if (bsize <= 0) {
		usrerr("size overflow during allocation");
		return (NULL);
	}
 /*
  * allocate one big hunk of memory for data + dope vectors
  */
	array = ecalloc(bsize, sizeof(char));
	if (array == NULL) {
		return (NULL);
	}

 /*
  * Why are we filling calloced memory with zeros?  This is a complete waste,
  * so is commented out.
  */

 /*
	bzero((char *) array, bsize);
  */

 /*
  * if 1-D array then all done
  */
	if (ndim < 2) {
		return (array);
	}
 /*
  * initialize the dope vectors
  */
 /* NOSTRICT */
	va_start(ap);

 /* NOSTRICT */
	(void) va_arg(ap, int);
 /* NOSTRICT */
	(void) va_arg(ap, int);

 /* NOSTRICT */
	dim = va_arg(ap, int);
	bsize = 0;
	size = 1;

	i = ndim - 1;
	do {
		REG_1 addr_t   *dope_p;	/* -> current dope vector	 */
		REG_5 int       j;	/* loop counter			 */
		REG_2 addr_t    next;	/* -> next subarray		 */

 /* NOSTRICT */
		dope_p = (addr_t *) (array + bsize);

		size *= dim;
		bsize += size * sizeof(addr_t);
		next = array + bsize;
 /* NOSTRICT */
		dim = va_arg(ap, int);

		j = size;
		do {
			*dope_p++ = next;
			next += dim * (i == 1 ? elsize : sizeof(addr_t));
		} while (--j > 0);
	} while (--i > 0);

	va_end(ap);
 /*
  * all done
  */
	return (array);
}

#endif  /* not STANDARD_C */
