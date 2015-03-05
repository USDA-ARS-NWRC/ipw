/*
** NAME
** 	read_headers  -- read input headers for sunweights
** 
** SYNOPSIS
**	void read_headers (fdg, fdm, nlines, nsamps, blat, blon, dlat, dlon)
**	int fdg, fdm;
**	int *nlines, *nsamps;
**	BIH_T **i_bih;
**	GEOH_T **i_geoh;
**	double *blat, *blon, *dlat, *dlon;
** 
** DESCRIPTION
**	read_headers reads the headers in the input header file for
**	sunweights.
** 
*/

#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "geoh.h"
#include "lqh.h"
#include "pgm.h"

char *units[] =
{
	(char *) NULL,
	"cosZ",
	"degrees"
};

static BIH_T         **i_bih;		/* -> geoh BIH			 */
static GEOH_T        **i_geoh;		/* -> lat/lon GEOH		 */

void
read_headers (
	int             fdg,		/* input geoh file descriptor	 */
	int             fdm,		/* mask file descriptor		 */
	int	       *nlines,		/* # lines in output image	 */
	int	       *nsamps,		/* # samples in output image	 */
	double	       *blat,		/* return UL latitude		 */
	double	       *blon,		/* return UL longitude (deg)	 */
	double	       *dlat,		/* latitude spacing (deg)	 */
	double	       *dlon)		/* longitude spacing (deg)	 */
{
	BIH_T         **m_bih;		/* -> mask BIH			 */
	float		elat;		/* ending latitude		 */
	float		elon;		/* ending longitude		 */

	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_geoh, 0};


   /* read BIH of GEOH file */

	if ((i_bih = bihread(fdg)) == NULL) {
		error ("can't read geoh BIH");
	}

	*nsamps = bih_nsamps (i_bih[0]);
	*nlines = bih_nlines (i_bih[0]);

   /* read mask BIH */
   /* make sure it has same dimensions of given geoh */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error ("can't read BIH of mask image");
		}
		if (*nsamps != bih_nsamps(m_bih[0]) ||
   	    	    *nlines != bih_nlines(m_bih[0])) {
			error ("geoh and mask images have different dimensions");
		}
		if (bih_nbands(m_bih[0]) != 1) {
			error ("mask image must have one band only");
		}
		skiphdrs (fdm);
	}

   /* ingest GEOH; other headers are skipped */

	gethdrs (fdg, request, NO_COPY, ERROR);

   /* check geo header - make sure image has lat/lon projection */

	if ((i_geoh = (GEOH_T **) hdr_addr (h_geoh)) != NULL) {
		*blat = geoh_bline (i_geoh[0]);
		*dlat = geoh_dline (i_geoh[0]);
		elat = GEO_LINE (i_geoh[0], *nlines-1);
		if (*dlat >= 0.0) {
			error ("Illegal dline in geo header - must be negative");
		}
		if (*blat > 90.0 || elat < -90.0) {
			error ("Illegal latitudes in GEO header");
		}

		*blon = geoh_bsamp (i_geoh[0]);
		*dlon = geoh_dsamp (i_geoh[0]);
		elon = GEO_SAMP (i_geoh[0], *nsamps-1);
		if (*dlon <= 0.0 || *blon < -180.0 || *blon > 180.0 ||
		    (elon - *blon) > 360.0) {
			error ("Illegal longitudes in GEO header");
		}
	} else {
		error ("input image has no geodetic header - must be lat/lon projection");
	}

}

/*
** NAME
** 	write_headers  -- write output image headers for sunweights
** 
** SYNOPSIS
**	void write_headers (fdo, nbits, mins, maxs)
**	int fdo;
**	int *nbits;
**	double *mins, *maxs;
** 
** DESCRIPTION
**	write_headers writes the headers to an output image for sunweights.
** 
** RESTRICTIONS
** 
** RETURN VALUE
** 
** GLOBALS ACCESSED
** 
** ERRORS
** 
** WARNINGS
** 
** APPLICATION USAGE
** 
** FUTURE DIRECTIONS
** 
** BUGS
**
*/
void
write_headers (
	int		fdo,		/* output image file desc	 */
	int	       *nbits,		/* # bits/band in output image	 */
	double	       *mins,		/* min value for each band	 */
	double	       *maxs)		/* max value for each band	 */
{
	int             band;		/* band loop counter		 */
	pixel_t         ibkpt[OBANDS];	/* integer break points	 	 */
	fpixel_t        fbkpt[OBANDS];	/* FP break points		 */
	BIH_T         **o_bih;		/* output image BIH		 */
	LQH_T         **o_lqh;		/* output image LQH		 */
	GEOH_T        **o_geoh;		/* output image GEOH		 */


   /* create output BIH (copy of input BIH), # bytes will be set by bihmake */

	o_bih = (BIH_T **) hdralloc (OBANDS, sizeof(BIH_T *), fdo, BIH_HNAME);
	for (band = 0; band < OBANDS; band++) {
		o_bih[band] = bihmake (0, nbits[band],
				   NULL, NULL,
				   o_bih[0],
				   bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
				   OBANDS);
	}

   /* write output BIH */

	if (bihwrite (fdo, o_bih) == ERROR) {
		error ("Can't write output BIH");
	}

   /* duplicate input GEO header and write to output image */

	if (i_geoh != NULL) {
		o_geoh = (GEOH_T **) hdralloc (OBANDS, sizeof(GEOH_T *), fdo,
			GEOH_HNAME);
		for (band = 0; band < OBANDS; band++) {
			o_geoh[band] = i_geoh[0];
		}
		if (geohwrite (fdo, o_geoh) == ERROR) {
			error ("Can't write output GEOH");
		}
	}


   /* allocate LQH */

	o_lqh = (LQH_T **) hdralloc (OBANDS, sizeof(LQH_T *), fdo, LQH_HNAME);

   /* create LQH for each band */

	for (band = 0; band < OBANDS; band++) {
		ibkpt[0] = 0;
		ibkpt[1] = ipow2 (nbits[band]) - 1;
		fbkpt[0] = mins[band];
		fbkpt[1] = maxs[band];
		if (mins[band] == maxs[band]) {
			fbkpt[1] = maxs[band] + 1;
		}
		o_lqh[band] = lqhmake (hnbits(fdo, band), 2,
				ibkpt, fbkpt, units[band], (char *) NULL);
	}

   /* write new LQH */

	if (lqhwrite (fdo, o_lqh) == ERROR) {
		error ("error writing new LQH");
	}

   /* prepare for image output */

	if (boimage (fdo) == ERROR) {
		error ("can't terminate header output");
	}
}
