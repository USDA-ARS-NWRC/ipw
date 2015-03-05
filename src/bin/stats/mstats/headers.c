#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(void)
{
 /*
  * need to read the LQ headers to initialize fpio
  */
 /* NOSTRICT */
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, NULL};

	BIH_T         **c_bihpp;	/* -> class BIH array		 */
	BIH_T         **i_bihpp;	/* -> input BIH array		 */

 /*
  * read input BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	parm.nlines = bih_nlines(i_bihpp[0]);
	parm.nsamps = bih_nsamps(i_bihpp[0]);
	parm.i_nbands = bih_nbands(i_bihpp[0]);
 /*
  * process remaining input headers
  */
	gethdrs(parm.i_fd, request, NO_COPY, ERROR);
 /*
  * return now if no class file
  */
	if (parm.c_fd == ERROR) {
		parm.c_nclasses = 1;
		return;
	}
 /*
  * read class BIH
  */
	c_bihpp = bihread(parm.c_fd);
	if (c_bihpp == NULL) {
		error("can't read basic image header");
	}
 /*
  * check class image sizes
  */
	if (parm.nlines != bih_nlines(c_bihpp[0])) {
		error("input and class image must have same # lines");
	}

	if (parm.nsamps != bih_nsamps(c_bihpp[0])) {
		error("input and class image must have same # samples");
	}

	if (bih_nbands(c_bihpp[0]) != 1) {
		error("class image must have only 1 band");
	}

	parm.c_nclasses = ipow2(bih_nbits(c_bihpp[0]));
 /*
  * skip remaining class headers
  */
	skiphdrs(parm.c_fd);
}
