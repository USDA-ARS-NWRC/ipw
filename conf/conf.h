/* conf.h.  Generated from conf.h.in by configure.  */
/*
 *  The local configuration include file for IPW.
 */


/*
 *  For backward compatability, make sure these unsigned typedefs are
 *  defined either in system include files, or with default values.
 */
#define uchar_t unsigned char
#define ushort_t unsigned short
#define ulong_t unsigned long

/*
 *  Big-endian or little-endian?
 */
#define WORDS_BIGENDIAN 0

#define SIZEOF_INT 4
#define SIZEOF_LONG 8
#define SIZEOF_SHORT 2

/*
 *  The header file specific to the type of host OS we're on.
 */
#include "host.h"

/*
 *  The header file specific to the local site
 */
#include "site.h"
