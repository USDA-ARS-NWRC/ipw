/*
** NAME
** 	headers -- read headers for imgstat
** 
** SYNOPSIS
**	void headers (i_fd, m_fd, class_stats, nclass, classes, units)
**	int i_fd, m_fd;
**	int class_stats;
**	int *nclass;
**	CLASS **classes;
**	char **units;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the given input image
**	and mask image, if specified.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"
#include "crh.h"
#include "pgm.h"


void
headers(
	int		i_fd,		/* input image file descriptor	 */
	int		m_fd,		/* mask image file descriptor	 */
	int		class_stats,	/* flag for class statistics	 */
	int  	       *nclass,		/* returned # classes in CRH	 */
	CLASS	      **classes,	/* class definitions in CRH	 */
	char	      **units)		/* units description in LQH	 */
{
	int		nlines;		/* #lines in input image	 */
	int		nsamps;		/* #samps in input image	 */
	int		nbands;		/* #bands in input image	 */
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	CRH_T         **i_crhpp;	/* -> input CRH array		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_crh = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T *request[] = {&h_lqh, &h_crh, NULL};


   /* read BIH */

	i_bihpp = bihread (i_fd);
	if (i_bihpp == NULL) {
		error ("can't read basic image header");
	}

	nlines = bih_nlines (i_bihpp[0]);
	nsamps = bih_nsamps (i_bihpp[0]);
	nbands = bih_nbands (i_bihpp[0]);
	if (class_stats) {
		if (nbands != 2) {
			error ("image has %d bands; image must have 2 bands",
			       nbands);
		}
	} else {
		if (nbands > 1) {
			error ("image has %d bands; single band images only",
			       nbands);
		}
	}

   /* process LQH header and CRH header */

	gethdrs (i_fd, request, NO_COPY, ERROR);

   /* get # classes if class stats requested */

	if (class_stats) {
		if ((i_crhpp = (CRH_T **) hdr_addr (h_crh)) == NULL) {
			warn ("can't read class-range header");
			*nclass = ipow2(bih_nbits(i_bihpp[0]));
			classes = NULL;
		} else {
			*nclass = crh_nclass (i_crhpp[0]);
			*classes = crh_class (i_crhpp[0]);
		}
	}

    /* otherwise, fetch the units */

	else {
		*units = lqh_units ( ((LQH_T **) hdr_addr(h_lqh))[0] );
	}

   /* process mask image headers */

	if (m_fd != ERROR) {
		if (bihread (m_fd) == NULL) {
			error ("can't read mask basic image header");
		}
		skiphdrs (m_fd);
		if (hnbands(m_fd) != 1) {
			error ("mask image has multiple bands");
		}
		if (nlines != hnlines(m_fd) ||
		    nsamps != hnsamps(m_fd)) {
			error ("input image and mask image are incompatible");
		}
	}
}
