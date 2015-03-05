/*
** NAME
** 	headers  -- read input header/write output images headers for reproj
** 
** SYNOPSIS
**	void
**	headers (fdi, fdg, fdm, fdo, o_nlines, o_nsamps, o_proj, o_units,
**		 o_bline, o_bsamp, o_dline, o_dsamp,
**	         i_bline, i_bsamp, i_dline, i_dsamp)
**	int fdi, fdg, fdm, fdo;
**	int o_nlines, o_nsamps;
**	int o_proj, o_units;
**	double *o_bline, *o_bsamp, *o_dline, *o_dsamp;
**	double *i_bline, *i_bsamp, *i_dline, *i_dsamp;
** 
** DESCRIPTION
**	Headers reads/ingests/checks the input, geodetic header and mask image
**	headers	and creates/writes the headers of the output image for the
**	program	reproj.
** 
**
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "pgm.h"
#include "gctp.h"

void
headers(
	int             fdi,		/* input image file descriptor	 */
	int             fdg,		/* geoh file descriptor		 */
	int             fdm,		/* mask file descriptor		 */
	int             fdo,		/* output image file descriptors */
	int	        o_nlines,	/* # lines in output image	 */
	int	        o_nsamps,	/* # samples in output image	 */
	int		o_proj,		/* projection ID for output image*/
	int		o_units,	/* projection name for output img*/
	double	       *o_bline,	/* output image UL geo line coord*/
	double	       *o_bsamp,	/* output image UL geo samp coord*/
	double	       *o_dline,	/* output image geo line incr	 */
	double	       *o_dsamp,	/* output image geo sample incr  */
	double	       *i_bline,	/* input image UL geo line coord */
	double	       *i_bsamp,	/* input image UL geo samp coord */
	double	       *i_dline,	/* input image geo line incr	 */
	double	       *i_dsamp)	/* input image geo samp incr	 */
{
	int		nbands;		/* # bands in images		 */
	BIH_T         **i_bih;		/* -> BIH of input image	 */
	LQH_T         **i_lqh;		/* -> LQH of input image	 */
	GEOH_T        **i_geoh;		/* -> GEOH of input image	 */
	BIH_T         **g_bih;		/* -> BIH of geoh file		 */
	GEOH_T        **g_geoh;		/* -> GEOH of geoh file		 */
	BIH_T         **m_bih;		/* -> mask BIH			 */
	BIH_T         **o_bih;		/* -> output BIH		 */
	LQH_T         **o_lqh;		/* -> output LQH	 	 */
	GEOH_T        **o_geoh;		/* -> output GEOH	 	 */
	int		band;		/* band loop counter		 */
	char	       *csys;		/* coord sys for output image	 */
	char	       *units;		/* units sys for output image	 */

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *i_request[] = {&h_lqh, &h_geoh, 0};
	static GETHDR_T gh_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *g_request[] = {&gh_geoh, 0};



   /* read input image BIH */

	if ((i_bih = bihread(fdi)) == NULL) {
		error ("can't read input BIH");
	}

   /* single-band images only */

	nbands = bih_nbands (i_bih[0]);
	if (nbands > 1) {
		error ("single-band input images only");
	}

   /* read mask headers */
   /* make sure it has same dimensions as input image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (bih_nsamps(i_bih[0]) != bih_nsamps(m_bih[0]) ||
       	    	    bih_nlines(i_bih[0]) != bih_nlines(m_bih[0])) {
			error ("input and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* read headers of input geoh file, if provided */

	if (fdg != ERROR) {
		if ((g_bih = bihread(fdg)) == NULL) {
			error ("can't read geoh BIH");
		}

		o_nlines = bih_nlines (g_bih[0]);
		o_nsamps = bih_nsamps (g_bih[0]);

		gethdrs (fdg, g_request, NO_COPY, ERROR);

		g_geoh = (GEOH_T **) hdr_addr (gh_geoh);
		if (g_geoh == NULL) {
			error ("error reading GEOH from given header file");
		}
		*o_bline = geoh_bline (g_geoh[0]);
		*o_bsamp = geoh_bsamp (g_geoh[0]);
		*o_dline = geoh_dline (g_geoh[0]);
		*o_dsamp = geoh_dsamp (g_geoh[0]);
	}

   /* create output BIH, # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (nbands, sizeof(BIH_T *), fdo, BIH_HNAME);

	for (band = 0; band < nbands; band++) {
		o_bih[band] = bihmake (0, bih_nbits(i_bih[band]),
			   	bih_history(i_bih[band]), bih_annot(i_bih[band]),
			   	o_bih[0], o_nlines, o_nsamps, nbands);
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* ingest input image LQH and GEOH; other headers are passed through */

	gethdrs (fdi, i_request, nbands, fdo);

   /* make sure image has geo header */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) != NULL) {
		*i_bline = geoh_bline (i_geoh[0]);
		*i_bsamp = geoh_bsamp (i_geoh[0]);
		*i_dline = geoh_dline (i_geoh[0]);
		*i_dsamp = geoh_dsamp (i_geoh[0]);
		if (*i_dline > 0.0) {
			warn ("dline > 0 in input image GEO header - northings should DECREASE");
		}
	} else {
		error ("input image has no geodetic header");
	}

	if (*o_dline > 0.0) {
		warn ("dline > 0 in output image GEO header - northings should DECREASE");
	}

   /* create and write geo header for output image */

	o_geoh = (GEOH_T **) hdralloc (nbands, sizeof(GEOH_T *), fdo, GEOH_HNAME);

	csys = get_proj_name (o_proj);
	units = get_proj_units (o_units);

	o_geoh[0] = geohmake (*o_bline, *o_bsamp, *o_dline, *o_dsamp, units, csys);
	for (band = 1; band < nbands; band++) {
		o_geoh[band] = i_geoh[0];
	}
	if (geohwrite(fdo, o_geoh) == ERROR) {
		error ("Can't write GEOH to output image");
	}

   /* duplicate input image LQH for output image */

	if ((i_lqh = (LQH_T **) hdr_addr (h_lqh)) != NULL) {

		o_lqh = (LQH_T **) lqhdup (i_lqh, nbands);
		if (o_lqh == NULL) {
			error ("Can't duplicate LQH of input image");
		}

		if (lqhwrite (fdo, o_lqh) == ERROR) {
			error ("Can't write output LQH");
		}
	} else {
		warn ("input image has no LQH; raw values used");
	}
		
   /* get ready for rest of output */

	if (boimage(fdo) == ERROR) {
		error ("Can't terminate header output");
	}
}
