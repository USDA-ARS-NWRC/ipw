/*
** name
**	headers -- read and write headers
**
** synopsis
**	void headers (fdi, fdm, fdo)
**	int fdi;
**	int fdm;
**	int fdo;
**
** description
**	headers reads the headers of the input and mask images and
**	write the BIH header of the output image.
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "gethdrs.h"
#include "fpio.h"
#include "pgm.h"


int N_BITS[] = {8};
    /*
     *  Number of bits for each band in output image.
     */


void
headers(
	int             fdi,		/* input file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
					/*   (== ERROR if no mask file)  */
	int             fdo)		/* output file descriptor	 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **m_bih;		/* mask BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
        int             band;           /* band loop counter             */


	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* ingest BIH of input image */

	if ((i_bih = bihread(fdi)) == NULL)
		error ("error reading BIH of input image");

   /* ingest BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL)
			error ("error reading BIH of mask image");
		if (bih_nlines(i_bih[0]) != bih_nlines(m_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]))
			error ("mask and input images have different dimensions");
		if (bih_nbands(m_bih[0]) != 1)
			error ("mask image must have only one band");
		skiphdrs (fdm);
	}

   /* make sure we've got the right number of bands in input image */

	if (hnbands(fdi) != IBANDS)
		error ("input image must have exactly %d bands", IBANDS);

   /* write output BIH */

	o_bih = (BIH_T **)
		hdralloc (OBANDS, sizeof(BIH_T *), fdo, BIH_HNAME);
	if (o_bih == NULL)
		error ("error allocating BIH for output image");

        for (band = 0; band < OBANDS; band++) {
                o_bih[band] = bihmake (0, N_BITS[band],
                                   bih_history(i_bih[0]), bih_annot(i_bih[0]),
                                   o_bih[0],
                                   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
                                   OBANDS);
        }

	if  (bihwrite (fdo, o_bih) == ERROR) {
		error ("can't write BIH for output image");
	}


   /* ingest LQH */

	gethdrs (fdi, request, OBANDS, fdo);

}
