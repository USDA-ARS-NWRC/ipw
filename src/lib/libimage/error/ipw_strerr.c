#include "ipw.h"
#include "ipw_errno.h"

/* ------------------------------------------------------------------------ */

/*
 *  Table of messages indexed by error codes
 */

static
char * ipw_err_strs[] = {

     /*                 0	*/"(no error)",

     /* IPWERR_MESSAGE	1	*/ NULL,

     /* IPWERR_MEMORY	2	*/"Insufficient memory			",

/*
 *  header I/O
 */
     /* IPWERR_RD_BIH	3	*/"Cannot read BI image header		",
     /* IPWERR_WR_BIH	4	*/"Cannot write BI header		",
     /* IPWERR_WR_BOI	5	*/"Cannot write image-data header	",
     /* IPWERR_WR_GEOH	6	*/"Cannot write GEO header		",
     /* IPWERR_WR_LQH	7	*/"Cannot write LQ header		"

};

/* ------------------------------------------------------------------------ */
 
char *
ipw_strerr(
	int	errno)		/* IPW error code	*/
{
	static char buffer[50];

	if ((errno < 0) || (errno >= sizeof(ipw_err_strs) / sizeof(char *))) {
		sprintf(buffer, "(unknown error code %d)", errno);
		return buffer;
	} else {
		return ipw_err_strs[errno];
	}
}
