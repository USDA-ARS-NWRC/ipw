#include "ipw.h"
#include "_hdrio.h"

/*
 *  These are the global variables internal to this directory.
 */


HDRIO_T *_hdriocb = NULL;

/*
 *	Dynamically-allocated array of image header I/O control blocks.
 *	Indexed by the corresponding UNIX file descriptor.
 */
