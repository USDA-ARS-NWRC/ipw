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
	if   (!already) { head1(); }
	else            { head2(); }
}

/*
 * first pass: read/write BIH; read/convert LQH
 */

void
head1(void)
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */
	int		firstbits;	/* nbits of first band		 */
	int		i;

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

	parm.ibands = hnbands(parm.i_fd);
	parm.obands = parm.ibands;

 /* allocate multiband lq minmax vectors */

        parm.mmval = (fpixel_t **) allocnd(sizeof(fpixel_t), 2,
                parm.obands, 2);
        if (parm.mmval == NULL) {
                error("can't allocate lq min/max buffer");
        }
 /*
  * if -p not specified, use maximum nbits among input bands
  *
  */
	if (parm.nbits == 0) {
	    firstbits = bih_nbits(i_bihpp[0]);
	    for(i = 0; i < parm.ibands; i++) {
                if (parm.nbits < bih_nbits(i_bihpp[i]))
                	parm.nbits = bih_nbits(i_bihpp[i]);
	    }
        }

 /*
  * create and write output BIH
  */
 /* NOSTRICT */
	o_bihpp = (BIH_T **) hdralloc(parm.obands, sizeof(BIH_T *), 
	    		parm.o_fd, BIH_HNAME);
	assert(o_bihpp != NULL);
	o_bihpp[0] = bihmake(0, parm.nbits,
	        bih_history(i_bihpp[0]), bih_annot(i_bihpp[0]), NULL,
		bih_nlines(i_bihpp[0]), bih_nsamps(i_bihpp[0]), parm.obands);

	for(i = 1; i < parm.obands; i++) {
	    o_bihpp[i] = bihmake(0, parm.nbits,
	        bih_history(i_bihpp[i]), bih_annot(i_bihpp[i]), o_bihpp[0],
		bih_nlines(i_bihpp[0]), bih_nsamps(i_bihpp[0]), parm.obands);
	    if (o_bihpp[i] == NULL) {
		error("can't make output BIH");
	    }
	}
	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * read the input LQH; other headers get passed
  */
	gethdrs(parm.i_fd, request, parm.obands, parm.o_fd);

}

void
head2(void)
{
	int		i;
	pixel_t         ival[2];	/* min, max pixel values	 */
	LQH_T         **o_lqhpp;	/* -> output LQH array		 */

 /*
  * make and write the output LQH
  */
	ival[0] = 0;
	ival[1] = ipow2(parm.nbits) - 1;
 /* NOSTRICT */
	o_lqhpp = (LQH_T **) hdralloc(parm.obands, sizeof(LQH_T *),
				      parm.o_fd, LQH_HNAME);
        if (o_lqhpp == NULL) {
                error("can't allocate output lq buffer");
        }

	for(i = 0; i < parm.obands; i++) {
	    if (parm.mmval[i][0] == parm.mmval[i][1])
		parm.mmval[i][1] += 1;
	    o_lqhpp[i] = lqhmake(parm.nbits, 2, ival,
		      &(parm.mmval[i][0]), (char *) NULL, (char *) NULL);
	}
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
