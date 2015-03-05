
#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers()
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */
	LQH_T         **i_lqhpp;	/* -> input LQH array		 */
	LQH_T         **h_lqhpp;	/* -> new LQH array		 */
	LQH_T         **o_lqhpp;	/* -> output LQH array		 */
	int             nbands;		/* # bands			 */
	int             band;		/* band index			 */

/* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}
	nbands = hnbands(parm.i_fd);
 /*
  * create output BIH
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}
 /*
  * get past the BIH of the header file
  */
	if (bihread(parm.h_fd) == NULL) {
		error("can't read dummy BIH of file with new LQH");
	}

	if (hnbands(parm.h_fd) != nbands) {
		error("input image and LQH file have different # bands");
	}
 /*
  * read new LQH
  */
	gethdrs(parm.h_fd, request, NO_COPY, parm.o_fd);
 /* NOSTRICT */
	if ((h_lqhpp = (LQH_T **) hdr_addr(h_lqh)) == NULL) {
		error("new LQH not valid");
	}
 /*
  * adjust nbits field of output image to correspond with new LQH
  */
	for (band = 0; band < nbands; ++band) {
		if (h_lqhpp[band] != NULL) {
			bih_nbits(o_bihpp[band]) = hnbits(parm.h_fd, band);
			bih_nbytes(o_bihpp[band]) = hnbytes(parm.h_fd, band);
		}
	}
 /*
  * write the output BIH
  */
	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write output BIH");
	}
 /*
  * read existing LQH of image, other headers get passed through
  */
	gethdrs(parm.i_fd, request, nbands, parm.o_fd);
 /*
  * reconcile the old and new LQH to create the output LQH
  */
 /* NOSTRICT */
	if ((i_lqhpp = (LQH_T **) hdr_addr(h_lqh)) == NULL) {
		o_lqhpp = h_lqhpp;
	}
	else {
		o_lqhpp = lqhdup(i_lqhpp, nbands);
		if (o_lqhpp == NULL) {
			error("can't duplicate input LQH");
		}

		for (band = 0; band < nbands; ++band) {
			if (h_lqhpp[band] != NULL) {
				if (o_lqhpp[band] != NULL) {
					LQH_T *lqhp = o_lqhpp[band];

					SAFE_FREE (lqhp->bkpt);
					SAFE_FREE (lqhp->map);
					SAFE_FREE (lqhp->units);
					SAFE_FREE (lqhp->interp);
					SAFE_FREE (lqhp->lininv);
					SAFE_FREE (lqhp);
				}
				o_lqhpp[band] = h_lqhpp[band];
			}
		}
	}
 /*
  * write the new LQH
  */
	if (lqhwrite(parm.o_fd, o_lqhpp) == ERROR) {
		error("can't write output LQH");
	}
 /*
  * done
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}
}
