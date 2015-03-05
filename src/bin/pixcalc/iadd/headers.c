#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(
	bool_t          already)
{
	if (!already) {
		head1();
	}
	else {
		head2();
	}
}

/*
 * first pass: read/write BIH; read/convert LQH
 */

void
head1()
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */
	int             band;		/* band index			 */
	int             nbands;		/* # input bands		 */

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
  * if nbits not specified on command line, use maximum among input
  * bands
  */
	if (parm.nbits <= 0) {
		for (band = nbands; --band >= 0;) {
			if (i_bihpp[band]->nbits > parm.nbits) {
				parm.nbits = i_bihpp[band]->nbits;
			}
		}
	}
 /*
  * create and write output BIH
  */
 /* NOSTRICT */
	o_bihpp = (BIH_T **) hdralloc(1, sizeof(BIH_T *), parm.o_fd, BIH_HNAME);
	assert(o_bihpp != NULL);
	o_bihpp[0] = bihmake(0, parm.nbits,
		       bih_history(i_bihpp[0]), bih_annot(i_bihpp[0]),
			     NULL,
		       bih_nlines(i_bihpp[0]), bih_nsamps(i_bihpp[0]),
			     1);
	if (o_bihpp == NULL) {
		error("can't make output BIH");
	}
	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * read the input LQH, band 0 of other headers get passed
  */
	gethdrs(parm.i_fd, request, 1, parm.o_fd);
 /*
  * convert lookup tables to negative for specified bands
  */
	if (parm.nsbands > 0) {
		fpixel_t      **map;	/* -> lookup tables	 */
		fpixel_t       *remap;	/* -> table to modify	 */
		int            *len;	/* -> lengths of tables	 */
		int             j;	/* index within table	 */

		assert((map = fpmap(parm.i_fd)) != NULL);
		assert((len = fpmaplen(parm.i_fd)) != NULL);

		for (band = 0; band < parm.nsbands; ++band) {
 /*
  * make sure this band exists
  */
			if (parm.sband[band] >= nbands) {
				error("-s %d: not that many bands",
				      parm.sband[band]);
			}
 /*
  * convert lookup table to negatives
  */
			remap = map[parm.sband[band]];
			for (j = len[parm.sband[band]]; --j >= 0;) {
				*remap *= -1.0;
				++remap;
			}
		}
	}
}

/*
 * write output LQH
 */

void
head2()
{
	pixel_t         ival[2];	/* min, max pixel values	 */
	LQH_T         **o_lqhpp;	/* -> output LQH array		 */

 /*
  * make and write the output LQH
  */
	ival[0] = 0;
	ival[1] = ipow2(parm.nbits) - 1;
 /* NOSTRICT */
	o_lqhpp = (LQH_T **) hdralloc(1, sizeof(LQH_T *),
				      parm.o_fd, LQH_HNAME);
	o_lqhpp[0] = lqhmake(parm.nbits, 2, ival,
		      &(parm.mmval[0]), (char *) NULL, (char *) NULL);
	if (lqhwrite(parm.o_fd, o_lqhpp) == ERROR) {
		error("can't write LQH");
	}
 /*
  * prepare to write image data
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}
}
