#include <math.h>
#include <strings.h>

#include "../skew.new/pgm.h"
#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "pixio.h"
#include "skewh.h"


/*
 * skew -- image horizontal skew/deskew
 */

void
run_skew(
		int             i_fd,		/* input image file descriptor	 */
		bool_t          fwd,		/* ? forward : inverse skew	 */
		double          angle,		/* skew angle			 */
		bool_t          skip_skewh,	/* ? skip input skew header	 */
		int				nthreads,	/* number of threads */
		int             o_fd)		/* output image file descriptor	 */
{
	/* NOSTRICT */
	static GETHDR_T h_skew = {SKEWH_HNAME, (ingest_t) skewhread};
	static GETHDR_T *hv[] = {&h_skew, NULL};

	int             band;		/* band counter			 */
	pixel_t        *buf;		/* -> I/O buffer		 */
	int             bufsiz;		/* size (# bytes) of I/O buffer	 */
	BIH_T         **i_bihpp;	/* -> input BIH			 */
	int             i_nsamps;	/* # samples / input line	 */
	fpixel_t        *image;		/* -> I/O buffer		 */
	int             line;		/* current line #		 */
	int             max_skew;	/* maximum skew (# samples)	 */
	int             nbands;		/* # image bands		 */
	bool_t          negflag;	/* ? angle < 0			 */
	int				Ni;			/* number of input pixels */
	int				No;			/* number of output pixels */
	int             nlines;		/* # image lines		 */
	fpixel_t 		*oimage;	/* output image */
	BIH_T         **o_bihpp;	/* -> output BIH		 */
	int             o_nsamps;	/* # samples / output line	 */
	int junk;
	int             scr_fd;		/* scratch file descriptor	 */
	char           *scr_name;	/* scratch file name		 */
	SKEWH_T       **skewhpp;	/* -> skew header		 */
	double          slope;		/* tan(angle)			 */

	/*
	 * read BIH
	 */
	i_bihpp = bihread(i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	nlines = bih_nlines(i_bihpp[0]);
	i_nsamps = bih_nsamps(i_bihpp[0]);
	nbands = bih_nbands(i_bihpp[0]);
	Ni = nlines * i_nsamps;

	/*
	 * forward skew: make skew header (there better not be one already)
	 */
	if (fwd) {
		if (got_hdr(h_skew) && !skip_skewh) {
			uferr(i_fd);
			error("image is already skewed");
		}
		/* NOSTRICT */
		skewhpp = (SKEWH_T **) hdralloc(nbands, sizeof(SKEWH_T *),
				ERROR, SKEWH_HNAME);
		if (skewhpp == NULL) {
			error("can't allocate skew header array");
		}

		skewhpp[0] = skewhmake(angle);
		if (skewhpp[0] == NULL) {
			error("can't make skew header");
		}

		for (band = 1; band < nbands; ++band) {
			skewhpp[band] = skewhpp[0];
		}
	}

	/*
	 * inverse skew: use existing skew header
	 */
	else {
		if (!got_hdr(h_skew)) {
			uferr(i_fd);
			error("image is not skewed");
		}
		/* NOSTRICT */
		skewhpp = (SKEWH_T **) hdr_addr(h_skew);

		for (band = 0; band < nbands; ++band) {
			if (skewhpp[0] == NULL) {
				uferr(i_fd);
				error("band %d has no skew header", band);
			}

			if (band == 0) {
				angle = skewh_angle(skewhpp[band]);
			}
			else if (skewhpp[band] != NULL &&
					angle != skewh_angle(skewhpp[band])) {
				uferr(i_fd);
				error("different skew angles: bands 0, %d",
						band);
			}
		}
	}

	/*
	 * calculate output line length
	 */
//	if (angle >= 0.0) {
//		negflag = FALSE;
//		slope = tan(angle * (M_PI / 180.0));
//	}
//	else {
//		negflag = TRUE;
//		slope = tan(-angle * (M_PI / 180.0));
//	}

	/*
	 ** can't find DTR function
	 **
	 **	slope = tan(DTR(angle));
	 */
//	max_skew = (nlines - 1) * slope + 0.5;

//	o_nsamps = i_nsamps;
//	if (fwd) {
//		o_nsamps += max_skew;
//		//		bufsiz = o_nsamps * nbands * sizeof(fpixel_t);
//	}
//	else {
//		o_nsamps -= max_skew;
//		assert(o_nsamps > 0);
//		//		bufsiz = i_nsamps * nbands * sizeof(fpixel_t);
//	}

	/*
	 * create output BIH
	 */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}
	gethdrs(i_fd, hv, nbands, o_fd);

	/*
	 * Allocate input/output images
	 */
	image = (fpixel_t *) ecalloc(Ni * nbands, sizeof(fpixel_t));
	if (image == NULL) {
		error("can't allocate input buffer");
	}

	//	oimage = (fpixel_t *) ecalloc(No * nbands, sizeof(fpixel_t));
	//	if (oimage == NULL) {
	//		error("can't allocate output buffer");
	//	}

	/*
	 * Read in image
	 */
	if (fpvread (i_fd, image, Ni) != Ni) {
		error ("input image read error");
	}

	/*
	 * calculate the skew
	 */
	skew(image, nlines, i_nsamps, nbands, fwd, angle, nthreads, &o_nsamps, &oimage);


	/*
	 * Write output BIH
	 */
	bih_nsamps(o_bihpp[0]) = o_nsamps;

	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}

	/*
	 * write skew header
	 */
	if (fwd) {
		if (skewhwrite(o_fd, skewhpp) == ERROR) {
			error("can't write skew header");
		}
	}

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}

	/*
	 * Write output file
	 */
	No = nlines * o_nsamps;
	if (fpvwrite(o_fd, oimage, No) != No) {
		error("error writing output file");
	}

	//	SAFE_FREE(buf);
	if (fwd) {
		SAFE_FREE(skewhpp[0]);
		SAFE_FREE(skewhpp);
	}
}
