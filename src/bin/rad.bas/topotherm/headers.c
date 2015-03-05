/*
**
** NAME
** 	headers -- read input image headers and write output image header
** 
** SYNOPSIS
** 
**	void headers (fdi, fdm, fdv, fdc, fdo, ibands, obands)
**	int fdi;
**	int fdm;
**	int fdv;
**	int fdc;
**	int fdo;
**	int ibands;
**	int obands;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the input images and
**	creates and writes header for the output image for pgm topotherm
** 
*/

#include        <math.h>
#include        "ipw.h"
#include        "bih.h"
#include        "lqh.h"
#include	"gethdrs.h"
#include        "pixio.h"
#include        "fpio.h"
#include        "envphys.h"
#include	"pgm.h"

void
headers (
	int             fdi,		/* input image file descriptor	*/
	int             fdm,		/* mask image file descriptor	*/
	int             fdv,		/* veg atten image file descriptor */
	int             fdc,		/* cloud atten image file descriptor */
	int             fdo,		/* output image file descriptor	*/
	int		ibands,		/* number of input bands	*/
	int		obands)		/* number of output bands	*/

{
	BIH_T         **i_bih;		/* -> input BIH			*/
	BIH_T         **m_bih;		/* -> mask BIH			*/
	BIH_T         **v_bih;		/* -> veg atten BIH		*/
	BIH_T         **c_bih;		/* -> cloud atten BIH		*/
	BIH_T         **o_bih;		/* -> output BIH		*/
	int             band;		/* band loop counter		*/

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};


   /* read BIH of input image */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* check input BIH */

	if (bih_nbands(i_bih[0]) != ibands) {
		printf("bih_nbands(i_bih[0]=%d, ibands=%d", bih_nbands(i_bih[0]), ibands);
		error ("not valid BIH for this program");
	}

   /* read BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read mask BIH");
		}

		if (bih_nlines(i_bih[0]) != bih_nlines(m_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]))
			error ("mask and input images are incompatible");

		if (bih_nbands(m_bih[0]) != 1)
			error ("mask image has > 1 bands");

		skiphdrs (fdm);
	}

   /* read BIH of veg atten image */

	if (fdv != ERROR) {
		if ((v_bih = bihread (fdv)) == NULL) {
			error ("can't read veg fact BIH");
		}

		if (bih_nlines(i_bih[0]) != bih_nlines(v_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(v_bih[0]))
			error ("veg fact and input images are incompatible");

		if (bih_nbands(v_bih[0]) != 1)
			error ("veg fact image has > 1 bands");

		skiphdrs (fdv);
	}

   /* read BIH of cloud atten image */

	if (fdc != ERROR) {
		if ((c_bih = bihread (fdc)) == NULL) {
			error ("can't read cloud atten BIH");
		}

		if (bih_nlines(i_bih[0]) != bih_nlines(c_bih[0]) ||
		    bih_nsamps(i_bih[0]) != bih_nsamps(c_bih[0]))
			error ("cloud atten and input images are incompatible");

		if (bih_nbands(c_bih[0]) != 1)
			error ("cloud atten image has > 1 bands");

		skiphdrs (fdv);
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

}
