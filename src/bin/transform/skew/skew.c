#include <math.h>
#include <strings.h>

#include "ipw.h"

#include "bih.h"
#include "gethdrs.h"
#include "pixio.h"
#include "skewh.h"

#include "pgm.h"

/*
 * skew -- image horizontal skew/deskew
 */

void
skew(
	int             i_fd,		/* input image file descriptor	 */
	bool_t          fwd,		/* ? forward : inverse skew	 */
	double          angle,		/* skew angle			 */
	bool_t          skip_skewh,	/* ? skip input skew header	 */
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
	int             line;		/* current line #		 */
	int             max_skew;	/* maximum skew (# samples)	 */
	int             nbands;		/* # image bands		 */
	bool_t          negflag;	/* ? angle < 0			 */
	int             nlines;		/* # image lines		 */
	BIH_T         **o_bihpp;	/* -> output BIH		 */
	int             o_nsamps;	/* # samples / output line	 */
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
 /*
  * construct scratch file name
  */
	scr_name = mktemplate(SCR_PREFIX);
	if (scr_name == NULL) {
		error("can't generate scratch file name");
	}
 /*
  * divert all input headers to scratch file; glom skew header if there is one
  */
	scr_fd = uwopen_temp(scr_name);
	if (scr_fd == ERROR) {
		error("can't open scratch file \"%s\"", scr_name);
	}

	gethdrs(i_fd, hv, nbands, scr_fd);

	if (boimage(scr_fd) == ERROR) {
		error("can't terminate header output");
	}

	if (uclose(scr_fd) == ERROR) {
		error("can't close scratch file");
	}
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
	if (angle >= 0.0) {
		negflag = FALSE;
	}
	else {
		negflag = TRUE;
		angle = -angle;
	}

/*
** can't find DTR function
**
**	slope = tan(DTR(angle));
*/
	slope = tan(angle * (M_PI / 180.0));
	max_skew = (nlines - 1) * slope + 0.5;

	o_nsamps = i_nsamps;
	if (fwd) {
		o_nsamps += max_skew;
		bufsiz = o_nsamps * nbands * sizeof(pixel_t);
	}
	else {
		o_nsamps -= max_skew;
		assert(o_nsamps > 0);
		bufsiz = i_nsamps * nbands * sizeof(pixel_t);
	}
 /*
  * create and write output BIH
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}

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
 /*
  * copy remaining headers from scratch file
  */
	scr_fd = uropen(scr_name);
	if (scr_fd == ERROR) {
		error("can't re-open scratch file \"%s\"", scr_name);
	}

	copyhdrs(scr_fd, nbands, o_fd);

	if (uclose(scr_fd) == ERROR) {
		error("can't close scratch file");
	}

	if (uremove(scr_name) == ERROR) {
		error("can't remove scratch file \"%s\"", scr_name);
	}
	SAFE_FREE(scr_name);

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}
 /*
  * allocate I/O buffer
  */
 /* NOSTRICT */
	buf = (pixel_t *) ecalloc(bufsiz, 1);
	if (buf == NULL) {
		error("can't allocate I/O buffer");
	}
 /*
  * process lines
  */
	for (line = 0; line < nlines; ++line) {
		pixel_t        *i_bufp;	/* -> beginning of input line	 */
		pixel_t        *o_bufp;	/* -> beginning of output line	 */
		int             offset;	/* read offset into buf		 */

		offset = (negflag ? line : nlines - line - 1)
			* nbands * slope + 0.5;

		if (fwd) {
			i_bufp = &buf[offset];
			o_bufp = buf;
		}
		else {
			i_bufp = buf;
			o_bufp = &buf[offset];
		}

		if (pvread(i_fd, i_bufp, i_nsamps) != i_nsamps) {
			error("image read failed, line %d", line);
		}

		if (pvwrite(o_fd, o_bufp, o_nsamps) != o_nsamps) {
			error("image write failed, line %d", line);
		}
 /* NOSTRICT */
		bzero((char *) buf, bufsiz);
	}

	SAFE_FREE(buf);
	if (fwd) {
		SAFE_FREE(skewhpp[0]);
		SAFE_FREE(skewhpp);
	}
}
