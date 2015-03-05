#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * headers -- process image headers
 */

void
headers(void)
{
	BIH_T         **i_bihpp;	/* -> input BIH array		 */
	BIH_T         **o_bihpp;	/* -> output BIH array		 */

 /*
  * read BIH
  */
	i_bihpp = bihread(parm.i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	parm.nbands = bih_nbands(i_bihpp[0]);
	parm.i_nlines = bih_nlines(i_bihpp[0]);
	parm.i_nsamps = bih_nsamps(i_bihpp[0]);
 /*
  * compute output image size
  */
	parm.o_nlines = parm.dup_lines > 1 ?
		parm.i_nlines * parm.dup_lines :
		(parm.i_nlines + parm.skip_lines - 1) / parm.skip_lines;

	parm.o_nsamps = parm.dup_samps > 1 ?
		parm.i_nsamps * parm.dup_samps :
		(parm.i_nsamps + parm.skip_samps - 1) / parm.skip_samps;
 /*
  * create and write BIH
  * 
  * NB: assume that there is only 1 copy of nlines,nsamps in the header; i.e. if
  * we change them for band 0 then they're changed for all bands.
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}

	bih_nlines(o_bihpp[0]) = parm.o_nlines;
	bih_nsamps(o_bihpp[0]) = parm.o_nsamps;

	if (bihwrite(parm.o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * if both zoom factors unity then just copy remaining headers
  */
	if (parm.skip_lines == 1 && parm.dup_lines == 1 &&
	    parm.skip_samps == 1 && parm.dup_samps == 1) {
		copyhdrs(parm.i_fd, parm.nbands, parm.o_fd);
	}
 /*
  * if {mag,min}ifying then diddle any spatial headers
  */
	else {
		fixhdrs();
	}
 /*
  * done with headers
  */
	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}
}
