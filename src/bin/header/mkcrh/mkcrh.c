/*
** NAME
** 	mkcrh - make a class range (CR) header for an image file
** 
** SYNOPSIS
**	mkcrh (i_fd, nclass, class, floor, ceil, annot, units, h_nbands,
**		h_band, force, o_fd)
**	int i_fd;
**	int nclass;
**	CLASS *class;
**	pixel_t floor, ceil;
**	char *annot;
**	char *units;
**	int h_nbands, *h_band;
**	bool_t force;
**	int o_fd;
** 
** DESCRIPTION
**	mkcrh creates a class range header for each of the specified bands
**	of the given image and writes it to the output file.  If force is
**	TRUE, then the input image is not copied to the output file.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "crh.h"
#include "pgm.h"

void
mkcrh (
	int             i_fd,		/* input image file descriptor	 */
	int             nclass,		/* # classes                     */
	CLASS	        class[],	/* array of class data values    */
	pixel_t		floor,		/* class floor value		 */
	pixel_t		ceil,		/* class ceil value		 */
	char           *annot,		/* annotation			 */
	char           *units,		/* units of measurement          */
	int             h_nbands,	/* # elements in h_band	 	 */
	int            *h_band,		/* -> bands to receive a CR hdr	 */
	bool_t          force,		/* don't copy input image        */
	int             o_fd)		/* input image file descriptor	 */
{
	static GETHDR_T h_cr = {CRH_HNAME, (ingest_t) crhread};
	static GETHDR_T *hv[] = {&h_cr, 0};

	BIH_T         **bihpp;		/* -> BI hdr array		 */
	int             i;		/* loop counter			 */
	CRH_T          *crhp;		/* -> new CR header		 */
	CRH_T         **crhpp;		/* -> input CR header array	 */
	int             nbands;		/* # image bands		 */
	int             nbits;		/* # bits / pixel		 */
	fpixel_t       *lo;		/* lo value for each class	 */
	fpixel_t       *hi;		/* hi value for each class	 */
	fpixel_t       *rep;		/* rep value for each class	 */


   /* read and write BIH */

	bihpp = bihread (i_fd);
	if (bihpp == NULL) {
		error ("can't read basic image header");
	}

	if (bihwrite (o_fd, bihpp) == ERROR) {
		error ("can't write basic image header");
	}

   /* extract goodies from BIH */

	nbands = bih_nbands(bihpp[0]);
	nbits = bih_nbits(bihpp[0]);
	if (hbit(floor) > nbits || hbit(ceil) > nbits || hbit(nclass-1) > nbits)
		warn ("not enough bits to hold all class values");

   /* check band #'s */

	for (i = 0; i < h_nbands; ++i) {
		if (h_band[i] < 0 || h_band[i] >= nbands) {
			uferr(i_fd);
			error("no band %d", h_band[i]);
		}
	}

   /* process remaining headers -- get CR header if there is one */

	gethdrs (i_fd, hv, nbands, o_fd);

   /* make CR header */

	lo = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));
	hi = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));
	rep = (fpixel_t *) ecalloc (nclass, sizeof(fpixel_t));

	for (i = 0; i < nclass; i++) {
		lo[class[i].cls-1] = class[i].lo;
		hi[class[i].cls-1] = class[i].hi;
		rep[class[i].cls-1] = class[i].rep;
	}

	crhp = crhmake (nclass, lo, hi, rep, floor, ceil, annot, units);
	if (crhp == NULL) {
		error ("can't create CR header");
	}

   /* if there's already an CR header then add to it */

	if (got_hdr(h_cr)) {
		crhpp = (CRH_T **) hdr_addr(h_cr);
	}

   /* if there's not already an CR header then create array */

	else {
		crhpp = (CRH_T **) hdralloc (nbands, sizeof(CRH_T *),
					    o_fd, CRH_HNAME);
		if (crhpp == NULL) {
			error ("can't allocate CR header array");
		}
	}

   /* insert new header in CR array */

	if (h_nbands < 1) {
		h_nbands = nbands;
	}

	for (i = 0; i < h_nbands; i++) {
		int             band;	/* current band #		 */

		band = (h_band == NULL) ? i : h_band[i];

		if (crhpp[band] != NULL) {
			uferr(i_fd);
			warn("replacing band %d CR header", band);
		}

		crhpp[band] = crhp;
	}

   /* write CR header */

	if (crhwrite(o_fd, crhpp) == ERROR) {
		error ("can't write CR header");
	}

   /* copy image data */

	if (boimage(o_fd) == ERROR) {
		error ("can't terminate header output");
	}

	if (!force) {
		if (imgcopy (i_fd, o_fd) == ERROR) {
			error ("can't copy image data");
		}
	}
}
