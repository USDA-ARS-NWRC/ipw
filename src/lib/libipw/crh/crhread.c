/*
** NAME
**	crhread -- read an IPW CRH header
**
** SYNOPSIS
**	#include "crh.h"
**
**	CRH_T **
**	crhread(
**		int             fd)	|* image file descriptor	 *|
**
** DESCRIPTION
**	crhread reads a CRH image header from file descriptor fd.  An array
**	of CRH_T pointers is allocated, one per band.  If a band has a CRH
**	header, then an CRH_T header is allocated and its address is placed
**	in the corresponding array element; otherwise, the corresponding
**	array element is NULL.
**
**	Before calling crhread, the caller must verify (by calling hrname())
**	that a CRH header is available for ingesting.
**
** RETURN VALUE
**	pointer to array of CRH_T pointers; else NULL if EOF or error
**
** GLOBALS ACCESSED
**	_crh
**
** ERRORS
**	can't allocate array of "cr" header pointers
**	"cr" header: bad band "{band}"
**	can't allocate "cr" header
**	"cr" header: key "{key}" has no value
**	"cr" header: bad key "{key}"
*/

#include <string.h>

#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "_crh.h"

CRH_T **
crhread(
	int             fd)		/* image file descriptor	 */
{
	CRH_T         **crhpp;		/* -> array of CRH pointers	 */
	CRH_T          *crhp =NULL;	/* -> current CRH		 */
	int             nbands;		/* # bands / pixel		 */
	int             band;		/* current header band #	 */
	int             err;		/* hgetrec return value		 */
	char           *hname;		/* current header name		 */
	int		nclass =0;	/* current # classes 		 */ 
	int		first_rec;	/* TRUE for first record read	 */


   /* allocate array of header pointers */

	nbands = hnbands (fd);
	assert(nbands > 0);

	crhpp = (CRH_T **) hdralloc(nbands, sizeof(CRH_T *), fd, CRH_HNAME);
	if (crhpp == NULL) {
		return (NULL);
	}

   /* loop through per-band headers */

	while ((hname = hrname(fd)) != NULL && streq(hname, CRH_HNAME)) {

		char            key[HREC_MAX + 1];	/* keyword	 */
		char            value[HREC_MAX + 1];	/* value string	 */

		first_rec = TRUE;

	/* get header band # */

		band = hrband(fd);
		if (band < 0 || band >= nbands) {
			uferr(fd);
			usrerr("\"%s\" header: bad band \"%d\"",
				CRH_HNAME, band);
			return (NULL);
		}

	/* allocate header */

		crhp = (CRH_T *) hdralloc(1, sizeof(CRH_T), fd, CRH_HNAME);
		if (crhp == NULL) {
			return (NULL);
		}


   	/* ingest records */

		while ((err = hgetrec(fd, (char *) NULL, key, value))
		       == HGOT_DATA) {

   		/* ignore all-comment records */

			if (key[0] == EOS) {
				continue;
			}

   		/* barf if missing value */

			if (value[0] == EOS) {
				uferr(fd);
				usrerr("\"%s\" header, key \"%s\": no value",
				       CRH_HNAME, key);
				return (NULL);
			}

		/* insist that first record is CRH_NCLASS key */

			if (first_rec) {
				first_rec = FALSE;
				if (!streq (key, CRH_NCLASS)) {
					usrerr(
					       "\"%s\" header: first key not \"%s\"",
					       CRH_HNAME, CRH_NCLASS);
					return (NULL);
				}

			/* allocate arrays */

				if (sscanf(value, "%d", &crhp->nclass) != 1) {
					usrerr(
					       "\"%s\" header: key \"%s\" missing value",
					       CRH_HNAME, key);
					return (NULL);
				}
				if (_crharrays(crhp) == ERROR) {
					return (NULL);
				}
				nclass = 0;

   		/* match key to header field, ingest value */

			} else if (streq(key, CRH_CLASS)) {
				fpixel_t	hi;
				fpixel_t	lo;
				fpixel_t	rep;

				if (nclass >= crhp->nclass) {
					usrerr(
					       "\"%s\" header: too many \"%s\" keys",
					       CRH_HNAME, key);
					return (NULL);
				}

				if (sscanf(value, "%f %f %f", &lo, &hi, &rep)
						!= 3) {
					usrerr(
					       "\"%s\" header: key \"%s\" must have exactly 3 values",
					       CRH_HNAME, key);
					return (NULL);
				}

				crhp->class[nclass].hi = hi;
				crhp->class[nclass].lo = lo;
				crhp->class[nclass].rep = rep;
				crhp->class[nclass].cls = nclass + 1;
				nclass++;
			} else if (streq(key, CRH_ANNOT)
				   && (crhp->annot = strdup(value)) == NULL) {
				return (NULL);
			} else if (streq(key, CRH_UNITS)
				   && (crhp->units = strdup(value)) == NULL) {
				return (NULL);
			} else if (streq(key, CRH_FLOOR)) {
				if (sscanf (value, "%d", &crhp->floor) != 1) {
					usrerr(
					       "\"%s\" header: key \"%s\" missing value",
					       CRH_HNAME, key);
					return (NULL);
				}
			} else if (streq(key, CRH_CEIL)) {
				if (sscanf (value, "%d", &crhp->ceil) != 1) {
					usrerr(
					       "\"%s\" header: key \"%s\" missing value",
					       CRH_HNAME, key);
					return (NULL);
				}
			}
		}

		crhpp[band] = crhp;

		if (err == ERROR) {
			return (NULL);
		}
	}

	if (hname == NULL) {
		return (NULL);
	}

	if (crhp->nclass != nclass) {
		usrerr(
		       "\"%s\" header: not enough \"%s\" keys", CRH_HNAME, CRH_NCLASS);
		return (NULL);
	}

   /* verify the header */

	if (!_crhcheck(crhpp, nbands)) {
		uferr(fd);
		return (NULL);
	}

	_crh[fd] = crhpp;
	return (crhpp);
}
