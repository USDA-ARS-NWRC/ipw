/*
** NAME
** 	headers -- process input and output headers for resample
** 
** SYNOPSIS
**	void
**	headers (fdi, fdm, fdo, zline, zsamp)
** 	int fdi, fdm, fdo;
**	int zline, zsamp;
** 
** DESCRIPTION
**	headers read and ingests the headers of the input images and 
**	creates and writes the headers of the output image to program
**	resample.
**
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "winh.h"
#include "gethdrs.h"

void
headers (
	int		fdi,		/* input image file descriptor	 */
	int		fdm,		/* mask image file descriptor	 */
	int		fdo,		/* output image file descriptor	 */
	int		zline,		/* line zoom factor		 */
	int		zsamp)		/* sample zoom factor		 */
{
	int		nbands;		/* # bands in input image	 */
	int		i_nlines;	/* # lines in input image	 */
	int		i_nsamps;	/* # sample in input image	 */
	int		o_nlines;	/* # lines in output image	 */
	int		o_nsamps;	/* # sample in output image	 */
	int             band;		/* band loop counter		 */
	BIH_T         **i_bih;		/* -> input BIH array		 */
	BIH_T         **m_bih;		/* -> mask BIH array		 */
	BIH_T         **o_bih;		/* -> output BIH array		 */
	LQH_T	      **o_lqh;		/* -> output LQH array		 */
	GEOH_T        **i_geoh;		/* -> input GEO header array	 */
	GEOH_T        **o_geoh =NULL;	/* -> output GEO header array	 */
	WINH_T        **winh;		/* -> WIN header array		 */

	static GETHDR_T h_lq = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_geo = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T h_win = {WINH_HNAME, (ingest_t) winhread};
	static GETHDR_T *hv[] = {&h_lq, &h_geo, &h_win, NULL};


   /* Read BIH */

	i_bih = bihread (fdi);
	if (i_bih == NULL) {
		error ("can't read basic image header");
	}

	nbands = bih_nbands (i_bih[0]);
	i_nlines = bih_nlines (i_bih[0]);
	i_nsamps = bih_nsamps (i_bih[0]);

   /* Read and check mask BIH */

	if (fdm != ERROR) {
		m_bih = bihread (fdm);
		if (i_bih == NULL) {
			error ("can't read basic image header");
		}
		if (bih_nbands (m_bih[0]) != 1)
			error ("mask image has > 1 bands");
		if ((bih_nlines (m_bih[0]) != i_nlines) ||
		    (bih_nsamps (m_bih[0]) != i_nsamps))
			error ("mask and input image have different dimensions");
		skiphdrs (fdm);
	}
   /* Compute output image size */

	o_nlines = (i_nlines + zline - 1) / zline;
	o_nsamps = (i_nsamps + zsamp - 1) / zsamp;

   /* Create and write output BIH */

	o_bih = (BIH_T **) hdralloc (nbands, sizeof(BIH_T *), fdo, BIH_HNAME);

	o_bih[0] = bihmake (0, bih_nbits (i_bih[0]),
			    bih_history(i_bih[0]), bih_annot(i_bih[0]), o_bih[0],
			    o_nlines, o_nsamps, nbands);

	if (o_bih[0] == NULL) {
		error ("Can't create BIH");
	}

	if (nbands > 0) {
		for (band = 1; band < nbands; band++) {
			o_bih[band] = bihmake (0, bih_nbits(i_bih[band]),
					bih_history(i_bih[band]),
					bih_annot(i_bih[band]),
					o_bih[0], bih_nlines(i_bih[0]),
					bih_nsamps(i_bih[0]), nbands);
		}
	}

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("can't write basic image header");
	}

   /* ingest LQ, GEO and/or WIN headers; copy all others */

	gethdrs (fdi, hv, nbands, fdo);

   /* copy LQ header if present */

	if (got_hdr(h_lq)) {
		o_lqh = lqhdup ((LQH_T **) hdr_addr(h_lq), nbands);
		if (o_lqh == NULL) {
			error ("can't duplicate lq header");
		}
		if (lqhwrite (fdo, o_lqh) == ERROR) {
			error ("can't write LQ header");
		}
	} else {
		warn ("no linear quantization in input image - raw values used");
	}

   /* adjust GEO header */

	if (got_hdr(h_geo)) {

		i_geoh = (GEOH_T **) hdr_addr(h_geo);

		o_geoh = geohdup (i_geoh, nbands);

		for (band = 0; band < nbands; band++) {
			if (o_geoh[band] != NULL) {
				(o_geoh[band]->bline) +=
				  (zline - 1) * o_geoh[band]->dline * 0.5;
				(o_geoh[band]->bsamp) +=
				  (zsamp - 1) * o_geoh[band]->dsamp * 0.5;
				(o_geoh[band]->dline) *= zline;
				(o_geoh[band]->dsamp) *= zsamp;
			}
		}

		if (geohwrite (fdo, o_geoh) == ERROR) {
			error ("can't write GEO header");
		}
	}

   /* adjust WIN header */

	if (got_hdr(h_win)) {

		winh = (WINH_T **) hdr_addr(h_win);

		for (band = 0; band < nbands; band++) {
			if (winh[band] != NULL) {
				winh_dline(winh[band]) *= zline;
				winh_dsamp(winh[band]) *= zsamp;
			}
		}

		if (winhwrite (fdo, winh) == ERROR) {
			error ("can't write WIN header");
		}
	}

   /* Terminate header output */

	if (boimage(fdo) == ERROR) {
		error("can't terminate header output");
	}

	(void) geohfree(o_geoh, nbands);
}
