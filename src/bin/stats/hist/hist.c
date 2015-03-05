#include "ipw.h"

#include "bih.h"
#include "pixio.h"
#include "gethdrs.h"

#include "pgm.h"

/*
 * hist -- compute image histogram
 */

void
hist(
	int             i_fd,		/* input file descriptor	 */
	int             m_fd,		/* mask file descriptor		 */
	int             h_fd)		/* histogram file descriptor	 */
{
	int             band;		/* band counter			 */
	BIH_T         **h_bihpp;	/* -> histogram header array	 */
	int             h_nsamps;	/* # samples / histogram line	 */
	hist_t        **histo;		/* -> histogram array		 */
	int             histsize;	/* total # bytes in hist	 */
	BIH_T         **i_bihpp;	/* -> input header array	 */
	pixel_t        *i_buf;		/* -> image input buffer	 */
	int             i_nbits;	/* # bits / input pixel		 */
	int             i_nlines;	/* # input lines		 */
	int             i_nsamps;	/* # samples / input line	 */
	int             line;		/* image line counter		 */
	pixel_t        *m_buf;		/* -> mask input buffer		 */
	int             nbands;		/* # bands			 */

 /*
  * process input headers
  */
	i_bihpp = bihread(i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	skiphdrs(i_fd);
 /*
  * make sure input bands have same size pixels (this is necessary since
  * o_nsamps = 2**i_nbits)
  */
	nbands = bih_nbands(i_bihpp[0]);
	i_nbits = bih_nbits(i_bihpp[0]);

	for (band = 1; band < nbands; ++band) {
		if (bih_nbits(i_bihpp[band]) != i_nbits) {
			uferr(i_fd);
			error("different size pixels: bands 0,%d", band);
		}
	}
 /*
  * process mask headers
  */
	i_nlines = bih_nlines(i_bihpp[0]);
	i_nsamps = bih_nsamps(i_bihpp[0]);

	if (m_fd != ERROR) {
		BIH_T         **m_bihpp;/* -> mask header array	 */

		m_bihpp = bihread(m_fd);
		if (m_bihpp == NULL) {
			error("can't read basic image header");
		}

		if (bih_nlines(m_bihpp[0]) != i_nlines ||
		    bih_nsamps(m_bihpp[0]) != i_nsamps ||
		    bih_nbands(m_bihpp[0]) != nbands) {
			uferr(m_fd);
			error("mask is not same size as input image");
		}

		skiphdrs(m_fd);
	}
 /*
  * allocate histogram array
  */
	h_nsamps = ipow2(i_nbits);
 /* NOSTRICT */
	histo = (hist_t **) allocnd(sizeof(hist_t), 2, h_nsamps, nbands);
	if (histo == NULL) {
		error("can't allocate histogram array");
	}
 /*
  * allocate I/O arrays
  */
 /* NOSTRICT */
	i_buf = (pixel_t *) ecalloc(i_nsamps * nbands, sizeof(pixel_t));
	if (i_buf == NULL) {
		error("can't allocate image input buffer");
	}

	if (m_fd == ERROR) {
		m_buf = NULL;
	}
	else {
 /* NOSTRICT */
		m_buf = (pixel_t *) ecalloc(i_nsamps * nbands,
					    sizeof(pixel_t));
		if (m_buf == NULL) {
			error("can't allocate mask input buffer");
		}
	}
 /*
  * read image and accumulate histogram
  */
	for (line = 0; line < i_nlines; ++line) {
		if (pvread(i_fd, i_buf, i_nsamps) != i_nsamps) {
			error("image read failed, line %d", line);
		}

		if (m_fd != ERROR) {
			if (pvread(m_fd, m_buf, i_nsamps) != i_nsamps) {
				error("mask read failed, line %d", line);
			}
		}

		histx(i_buf, m_buf, i_nsamps, nbands, histo);
	}

	SAFE_FREE(i_buf);
	SAFE_FREE(m_buf);

 /*
  * create and write histogram BIH
  */
 /* NOSTRICT */
	h_bihpp = (BIH_T **) hdralloc(nbands, sizeof(BIH_T *), h_fd,
				      BIH_HNAME);
	if (h_bihpp == NULL) {
		error("can't allocate pointers to basic image headers");
	}

	for (band = 0; band < nbands; ++band) {
		hist_t          h_max;
		int             samp;

		h_max = 0;

		for (samp = 0; samp < h_nsamps; ++samp) {
			if (histo[samp][band] > h_max) {
				h_max = histo[samp][band];
			}
		}

		h_bihpp[band] =
			bihmake(sizeof(hist_t),
				hbit((unsigned) h_max),
				bih_history(i_bihpp[0]),
				bih_annot(i_bihpp[0]),
				band == 0 ? (BIH_T *) NULL : h_bihpp[0],
				1, h_nsamps, nbands);

		if (h_bihpp[band] == NULL) {
			error("can't create basic image header for band %d",
			      band);
		}
	}

	if (bihwrite(h_fd, h_bihpp) == ERROR) {
		error("can't write histogram basic image header");
	}

	if (boimage(h_fd) == ERROR) {
		error("can't terminate header output");
	}
 /*
  * write histogram
  */
	histsize = h_nsamps * nbands * sizeof(hist_t);
 /* NOSTRICT */
	if (uwrite(h_fd, (addr_t) * histo, histsize) != histsize) {
		error("can't write histogram");
	}

	SAFE_FREE(histo);
}
