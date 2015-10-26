/*
 ** NAME
 ** 	headers -- read input image header and write output image header
 **
 ** SYNOPSIS
 **
 **	void headers (fdi, fdo, obands)
 **	int fdi, fdo;
 **	int obands;
 **
 ** DESCRIPTION
 ** 	headers reads and checks the headers of the input image and
 **	creates and writes headers for the output image of the snow albedo
 **	program ialbedo
 **
 */

#include "ipw.h"
#include "pgm.h"
#include "bih.h"
#include "lqh.h"
#include "fpio.h"
#include "gethdrs.h"

void
headers (
		int             fdi,		/* input image file descriptor	*/
		int             fdo,		/* output image file descriptor	*/
		int             fds,		/* storm image file descriptor	*/
		int		obands)		/* # output bands		*/
{
	BIH_T         **i_bih;		/* -> input BIH			*/
	BIH_T         **o_bih;		/* -> output BIH		*/
	BIH_T         **s_bih;		/* -> storm BIH			*/
	int             band;		/* band loop counter		*/

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


	/* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

	/* check input BIH */

	if (bih_nbands(i_bih[0]) != 1) {
		error ("input image has > 1 band");
	}

	/* create output BIH, # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (obands, sizeof(BIH_T *), fdo, BIH_HNAME);
	for (band = 0; band < obands; band++) {
		o_bih[band] = bihmake (0, CHAR_BIT,
				bih_history(i_bih[0]), bih_annot(i_bih[0]),
				o_bih[0],
				bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
				obands);
	}

	/* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

	/* ingest LQH, other headers get passed thru */

	gethdrs (fdi, request, obands, fdo);

	if (hdr_addr (h_lqh) == NULL)
		warn ("input file has no LQH; raw values used");

	/* read BIH of storm image */
	if (fds != ERROR) {
		if ((s_bih = bihread (fds)) == NULL) {
			error ("can't read storm BIH");
		}

		if (bih_nlines(i_bih[0]) != bih_nlines(s_bih[0]) ||
				bih_nsamps(i_bih[0]) != bih_nsamps(s_bih[0]))
			error ("storm and input images are incompatible");

		if (bih_nbands(s_bih[0]) != 1)
			error ("storm image has > 1 bands");

		//		skiphdrs (fds);

		/* ingest storm LQH, other headers are skipped */

		gethdrs(fds, request, NO_COPY, ERROR);

		if (hdr_addr(h_lqh) == NULL)
			warn("storm input file has no LQH; raw values used");

	}

}
