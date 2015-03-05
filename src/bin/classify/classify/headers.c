/*
** NAME
** 	headers -- read input headers/write output headers for classify
** 
** SYNOPSIS
**	headers (fdi, fdh, fdm, ifp, gotcrh, classes, nclass,
**		 floor, ceil, fdo)
** 	int fdi, fdh, fdm;
**	FILE *ifp;
**	int gotcrh;
**	CLASS *classes;
**	int nclass;
**	pixel_t *floor, *ceil;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the IPW input image
**	and initializes and writes headers for the output classified
**	image for the classify program.  The number of classes and lo
**	hi, and rep arrays are returned.
*/

#include "ipw.h"
#include "pgm.h"
#include "gethdrs.h"
#include "bih.h"
#include "crh.h"
#include "lqh.h"

void
headers (
	int		 fdi,		/* input image file descriptor	*/
	int		 fdh,		/* input CRH file descriptor	*/
	int		 fdm,		/* mask image file descriptor	*/
	FILE		*ifp,		/* -> input class file		*/
	int		 gotcrh,	/* TRUE if input CRH specified	*/
	CLASS		*classes,	/* lo,hi,rep,class# for each cls*/
	int		*nclass,	/* -> #classes in output image	*/
	int		*floor,		/* -> floor class (-1 if unset)	*/
	int		*ceil,		/* -> ceil class (-1 if unset)	*/
	int		 fdo)		/* output image file descriptor */
{
	int		 nbits;		/* # bits/samp in output image  */
	int		 i;		/* loop counter			*/
	char		*units;		/* -> units of data		*/
	fpixel_t	*lo;		/* lo value for each class	*/
	fpixel_t	*hi;		/* hi value for each class	*/
	fpixel_t	*rep;		/* rep value for each class	*/
	BIH_T          **i_bih;		/* -> input BIH			*/
	LQH_T	       **i_lqh;		/* -> input LQH			*/
	BIH_T          **m_bih;		/* -> mask BIH			*/
	BIH_T          **h_bih;		/* -> CRH BIH			*/
	CRH_T	       **i_crh =NULL;	/* -> input CRH			*/
	BIH_T          **o_bih;		/* -> output BIH		*/
	LQH_T          **o_lqh;		/* -> output LQH		*/
	CRH_T          **o_crh;		/* -> output CRH		*/
	pixel_t		ibkpt[2];	/* integer breakpoints for LQH	*/
	fpixel_t	fbkpt[2];	/* float breakpoints for LQH	*/

	static GETHDR_T h_crh = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_crhh = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T *request[] = {&h_crh, &h_lqh, 0};
	static GETHDR_T *requesth[] = {&h_crhh, 0};


   /* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* read BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read mask BIH");
		}
	}

   /* check input BIH */

	if (hnbands(fdi) != 1) {
		error ("not valid BIH for this program");
	}

	if (fdm != ERROR) {
		if (hnbands(fdm) != 1) {
			error ("not valid mask BIH for this program");
		}
		if (hnlines(fdi) != hnlines(fdm) ||
		    hnsamps(fdi) != hnsamps(fdm)) {
			error ("mask and input images are incompatible");
		}
	}

   /* read BIH and CRH of input CRH file */
   /* get class information */

	if (gotcrh) {
		if ((h_bih = bihread (fdh)) == NULL) {
			error ("can't read BIH from input CRH");
		}
		gethdrs (fdh, requesth, NO_COPY, ERROR);
		if ((i_crh = (CRH_T **) hdr_addr (h_crhh)) == NULL) {
			error ("can't read input CRH");
		}
		if (crhsort (i_crh[0], SORT_BY_RANGE) == ERROR) {
			error ("error sorting classes");
		}

		*nclass = crh_nclass (i_crh[0]);
		*floor = crh_floor (i_crh[0]);
		*ceil = crh_ceil (i_crh[0]);
		for (i = 0; i < *nclass; i++) {
			classes[i].lo = crh_lo (i_crh[0], i);
			classes[i].hi = crh_hi (i_crh[0], i);
			classes[i].rep = crh_rep (i_crh[0], i);
			classes[i].cls = crh_cls (i_crh[0], i);
		}

	} else {
		get_classes (ifp, nclass, classes);
		if (*floor == -1) {
			*floor = NO_DATA;
		} else if (*floor > *nclass) {
			error ("floor value must be <= # classes");
		}

		if (*ceil == -1) {
			*ceil = NO_DATA;
		} else if (*ceil > *nclass) {
			error ("ceil value must be <= # classes");
		}
	}

	if (fdm != ERROR) {
		skiphdrs (fdm);
	}

   /* create output BIH, # bytes will be set by bihmake */

	nbits = hbit(*nclass);

	o_bih = (BIH_T **) hdralloc (1, sizeof(BIH_T *), fdo, BIH_HNAME);

	o_bih[0] = bihmake (0, nbits,
			   bih_history(i_bih[0]), bih_annot(i_bih[0]), o_bih[0],
			   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]), 1);
	if (o_bih[0] == NULL) {
		error ("Can't create BIH");
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest CRH, LQH, other headers get passed thru */

	gethdrs (fdi, request, 1, fdo);

	if (hdr_addr (h_crh) != NULL) {
		warn ("input image already classified - reclassifying");
	}

	if ((i_lqh = (LQH_T **) hdr_addr (h_lqh)) != NULL) {
		units = lqh_units (i_lqh[0]);
	} else {
		units = NULL;
	}

   /* allocate LQH */

	o_lqh = (LQH_T **) hdralloc (hnbands(fdo), sizeof(LQH_T *),
				    fdo, LQH_HNAME);
	if (o_lqh == NULL) {
		error ("can't allocate new LQH");
	}

   /* create new LQH */

	ibkpt[0] = fbkpt[0] = 0;
	ibkpt[1] = fbkpt[1] = ipow2 (nbits) - 1;
	o_lqh[0] = lqhmake (nbits, 2, ibkpt, fbkpt, units, (char *) NULL);

   /* write new lqh */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

   /* create or duplicate CRH */

	if (gotcrh) {

		o_crh = crhdup (i_crh, 1);

	} else {

		o_crh = (CRH_T **) hdralloc (1, sizeof(CRH_T *), fdo,
					     CRH_HNAME);

		lo = (fpixel_t *) ecalloc (*nclass, sizeof(fpixel_t));
		hi = (fpixel_t *) ecalloc (*nclass, sizeof(fpixel_t));
		rep = (fpixel_t *) ecalloc (*nclass, sizeof(fpixel_t));

		for (i = 0; i < *nclass; i++) {
			lo[classes[i].cls-1] = classes[i].lo;
			hi[classes[i].cls-1] = classes[i].hi;
			rep[classes[i].cls-1] = classes[i].rep;
		}

		o_crh[0] = crhmake (*nclass, lo, hi, rep, *floor, *ceil,
				    (char *) NULL, units);
		if (o_crh[0] == NULL) {
			error ("can't create CRH");
		}

	}

   /* write output CRH */

	if (crhwrite (fdo, o_crh) == ERROR) {
		error ("Can't write output CRH");
	}

   /* get ready for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}
}
