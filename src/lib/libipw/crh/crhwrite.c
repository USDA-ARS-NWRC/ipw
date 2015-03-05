/*
** NAME
**	crhwrite -- write an IPW CRH header
**
** SYNOPSIS
**	#include "crh.h"
**
**	int
**	crhwrite(
**		int     fd,		|* output file descriptor	 *|
**		CRH_T **crhpp)		|* -> array of CRH pointers	 *|
**
** DESCRIPTION
**      crhwrite writes the array of CRH headers pointed to by crhpp
**	to file descriptor fd.
**
**	crhwrite is called by IPW application programs to write
**	CRH headers.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** HISTORY
**	Written by Kelly Longley, ERL-C.
**
** BUGS
**	crhsort expects a *CRH_T, while we pass in a *CLASS.  This is wrong,
**	but I'm not sure I want to fix it.  It should blow up, but it seems
**	to work, so I won't muck with it now.  Dana Jacobsen, ERL-C.
**
**	Well, I've decided to muck with it.  Hopefully, the fix won't blow
**	up.  J. Domingo, OSU, 29 May 1996
*/

#include <strings.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "_crh.h"

int
crhwrite(
	int             fd,		/* output file descriptor	 */
	CRH_T         **crhpp)		/* -> array of CRH pointers	 */
{
	REG_2 int       band;		/* current header band #	 */
	int             nbands;		/* # bands in output image	 */
	CRH_T		temp;		/* temp CRH for sorting classes  */

	nbands = hnbands(fd);

	assert(_crhcheck(crhpp, nbands));

   /* loop through possible output bands */

	for (band = 0; band < nbands; ++band) {
		int             i;	/* loop counter			 */
		REG_1 CRH_T    *crhp;	/* -> current CRH		 */
		char            s[HREC_SIZ];	/* output value		 */

		crhp = crhpp[band];
		if (crhp == NULL) {
			continue;
		}

		if (hwprmb(fd, CRH_HNAME, band, CRH_VERSION) == ERROR) {
			return (ERROR);
		}

   	/* write fields */

		(void) sprintf(s, "%d", crhp->nclass);
		if (hputrec(fd, (char *) NULL, CRH_NCLASS, s) == ERROR) { 
			return (ERROR);
		}

		(void) sprintf(s, "%d", crhp->floor);
		if (hputrec(fd, (char *) NULL, CRH_FLOOR, s) == ERROR) { 
			return (ERROR);
		}

		(void) sprintf(s, "%d", crhp->ceil);
		if (hputrec(fd, (char *) NULL, CRH_CEIL, s) == ERROR) { 
			return (ERROR);
		}

		/* copy CLASS array into temp CRH and sort by class numbers */

		temp.class = (CLASS *) ecalloc (crhp->nclass, sizeof(CLASS));
		if (temp.class == NULL) 
			error ("Unable to allocate class structure copy");

		bcopy((void *) crhp->class, (void *) temp.class, 
			(size_t) crhp->nclass * sizeof(CLASS));

		temp.nclass = crhp->nclass;
		temp.floor  = crhp->floor;
		temp.ceil   = crhp->ceil;
		temp.annot  = crhp->annot;
		temp.units  = crhp->units;

		crhsort (&temp, SORT_BY_CLASS);

		for (i = 0; i < crhp->nclass; i++) {
			(void) sprintf(s, "%.10g %.10g %.10g",
				       temp.class[i].lo,
				       temp.class[i].hi,
				       temp.class[i].rep);

			if (hputrec(fd, (char *) NULL, CRH_CLASS, s)
			    == ERROR) {
				return (ERROR);
			}
		}

		SAFE_FREE(temp.class);

		if (crhp->annot != NULL) {
			if (hputrec(fd, (char *) NULL, CRH_ANNOT, crhp->annot)
			    == ERROR) {
				return (ERROR);
			}
		}

		if (crhp->units != NULL) {
			if (hputrec(fd, (char *) NULL, CRH_UNITS, crhp->units)
			    == ERROR) {
				return (ERROR);
			}
		}


	}

	_crh[fd] = crhpp;
	return (OK);
}
