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
	int		band;		/* loop counter 		 */
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	parm.nbands = bih_nbands(i_bihpp[0]);
	parm.nsamps = bih_nsamps(i_bihpp[0]);
	parm.nlines = bih_nlines(i_bihpp[0]);

 /*
  * allocate, create, and write output BIH
  */

    o_bihpp = (BIH_T **) hdralloc (parm.nbands, sizeof(BIH_T *), parm.o_fd,
		BIH_HNAME);
 
    for(band = 0; band < parm.nbands; band++) {
            o_bihpp[band] = bihmake (0, parm.nbits,
    		        bih_history(i_bihpp[0]), bih_annot(i_bihpp[0]),
        	        o_bihpp[0], parm.nlines, parm.nsamps, parm.nbands);
    }

    if (o_bihpp == NULL) {
        error("can't make output BIH");
    }
    if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
        error("can't write basic image header");
    }

 /*
  * read and skip the input LQH; other headers get passed
  */
	gethdrs(parm.i_fd, request, parm.nbands, parm.o_fd);
}
