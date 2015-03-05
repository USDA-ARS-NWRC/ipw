/*
** NAME
** 	headers -- read input headers/write output headers for classeq
** 
** SYNOPSIS
**	headers (fdi, fdm, nclass, fdo)
** 	int fdi, fdm, fdo;
**	int nclass;
** 
** DESCRIPTION
** 	headers reads and checks the headers of the IPW input image
**	and initializes and writes headers for the output classified
**	image for the classeq program.
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "crh.h"
#include "lqh.h"
#include "fpio.h"

#include "pgm.h"

void
headers (
	int		fdi,		/* input image file descriptor	*/
	int		fdm,		/* mask image file descriptor	*/
	int		nclass,		/* # classes in output image    */
	int		fdo)		/* output image file descriptor */
{
	int		nbits;		/* # bits/samp in output image  */
	BIH_T         **i_bih;		/* -> input BIH			*/
	BIH_T         **m_bih;		/* -> mask BIH			*/
	BIH_T         **o_bih;		/* -> output BIH		*/
	LQH_T         **o_lqh;		/* -> output LQH		*/
	pixel_t		ibkpt[2];	/* integer breakpointf for LQH  */
	fpixel_t	fbkpt[2];	/* float breakpointf for LQH    */

	static GETHDR_T h_crh = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_crh, &h_lqh, 0};


   /* read BIH of input and mask images */

	if ((i_bih = bihread (fdi)) == NULL) {
		error ("can't read input BIH");
	}

	if (fdm != ERROR) {
		if ((m_bih = bihread (fdm)) == NULL) {
			error ("can't read mask BIH");
		}
	}

   /* check input and mask BIH */

	if (hnbands(fdi) != 1) {
		error ("not valid BIH for this program");
	}

	if (fdm != ERROR) {
		if (hnbands(fdm) != 1) {
			error ("not valid mask BIH for this program");
		}
		if (hnsamps(fdi) != hnsamps(fdm) ||
	    	hnlines(fdi) != hnlines(fdm)) {
			error (
			    "input and mask images have different dimensions");
		}
	}

   /* create output BIH, # bytes will be set by bihmake */

	nbits = hbit (nclass);
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
	if (fdm != ERROR)
		fphdrs (fdm, NO_BAND, ERROR);

	if (hdr_addr (h_crh) == NULL) {
	} else {
		warn ("input image already classified - reclassifying");
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
	o_lqh[0] = lqhmake (nbits, 2, ibkpt, fbkpt, (char *) NULL, (char *) NULL);

   /* write new lqh */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("Can't write output LQH");
	}

}
