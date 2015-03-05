#include "ipw.h"

#include "bih.h"
#include "geoh.h"
#include "gethdrs.h"
#include "winh.h"

#include "pgm.h"

/*
 * window -- extract window from image
 */

void
window(
	int             i_fd,		/* input image file descriptor	 */
	XWSPEC_T       *xwp,		/* -> command line window spec.	 */
	int             o_fd)		/* output image file descriptor	 */
{
	static GETHDR_T h_geo = {
 /* NOSTRICT */
		GEOH_HNAME, (ingest_t) geohread
	};

	static GETHDR_T h_win = {
 /* NOSTRICT */
		WINH_HNAME, (ingest_t) winhread
	};

	static GETHDR_T *hv[] = {
		&h_geo,
		&h_win,
		0
	};

	int             band;		/* current header band #	 */
	char           *buf;		/* -> I/O buffer		 */
	long            bskip;		/* # bytes to skip at BO line	 */
	long            eskip;		/* # bytes to skip at EO line	 */
	GEOH_T        **geohpp;		/* -> geodetic header		 */
	BIH_T         **i_bihpp;	/* input BIH			 */
	int             i_linesize;	/* # bytes / input line		 */
	int             i_nlines;	/* # input lines		 */
	int             i_nsamps;	/* # samples / input line	 */
	int             i_sampsize;	/* # bytes / input sample	 */
	int             line;		/* current input line #		 */
	int             nbands;		/* # input bands		 */
	BIH_T         **o_bihpp;	/* output BIH			 */
	int             o_linesize;	/* # bytes / output line	 */
	int             scr_fd;		/* scratch file descriptor	 */
	char           *scr_name;	/* scratch file name		 */
	WINH_T         *winhp;		/* -> window header		 */
	WINH_T        **winhpp;		/* -> window header array	 */
	WSPEC_T        *wp;		/* intrinsic window spec.	 */

 /*
  * read BIH
  */
	i_bihpp = bihread(i_fd);
	if (i_bihpp == NULL) {
		error("can't read basic image header");
	}

	i_nlines = bih_nlines(i_bihpp[0]);
	i_nsamps = bih_nsamps(i_bihpp[0]);
	nbands = bih_nbands(i_bihpp[0]);
 /*
  * create scratch file for header diversion
  */
	scr_name = mktemplate(SCR_PREFIX);
	if (scr_name == NULL) {
		error("can't generate scratch file name");
	}

	scr_fd = uwopen_temp(scr_name);
	if (scr_fd == ERROR) {
		error("can't open scratch file \"%s\"", scr_name);
	}
 /*
  * divert remaining input headers; shortstop any win or geo headers
  */
	gethdrs(i_fd, hv, nbands, scr_fd);

	if (boimage(scr_fd) == ERROR) {
		error("can't terminate scratch file header output");
	}

	if (uclose(scr_fd) == ERROR) {
		error("can't close scratch file \"%s\"", scr_name);
	}
 /*
  * convert extrinsic to intrinsic window spec
  */
 /* NOSTRICT */
	winhpp = (WINH_T **) hdr_addr(h_win);
 /* NOSTRICT */
	geohpp = (GEOH_T **) hdr_addr(h_geo);

	wp = cvt_wspec(xwp, nbands, winhpp, geohpp);
	if (wp == NULL) {
		uferr(i_fd);
		error("can't convert window to intrinsic coordinates");
	}
 /*
  * check window spec against input BIH, adjust if necessary
  */
	if (fix_wspec(wp, i_nlines, i_nsamps) == ERROR) {
		uferr(i_fd);
		error("bad window specification");
	}
 /*
  * reset o_byteorder from hostorder() to byte order of i_fd
  * because this program will bypass the pixio layer
  */
	o_byteorder = hbyteorder(i_fd);
 /*
  * create and write BIH
  */
	o_bihpp = bihdup(i_bihpp);
	if (o_bihpp == NULL) {
		error("can't create basic image header");
	}

	bih_nlines(o_bihpp[0]) = wp->nlines;
	bih_nsamps(o_bihpp[0]) = wp->nsamps;

	if (bihwrite(o_fd, o_bihpp) == ERROR) {
		error("can't write basic image header");
	}
 /*
  * allocate window header pointer array if didn't read one
  */
	if (winhpp == NULL) {
 /* NOSTRICT */
		winhpp = (WINH_T **) hdralloc(nbands, sizeof(WINH_T *),
					      o_fd, WINH_HNAME);
		if (winhpp == NULL) {
			error("can't allocate window header pointer array");
		}
	}

	for (band = 0; band < nbands; ++band) {
 /*
  * supply default window header if none for this band
  */
		if (winhpp[band] == NULL) {
			winhp = winhmake((double) wp->bline, (double) wp->bsamp, 1.0, 1.0);
			if (winhp == NULL) {
				error("can't make window header");
			}
			winhpp[band] = winhp;
		}
 /*
  * adjust existing window header
  */
		else {
			winh_bline(winhpp[band]) +=
				wp->bline * winh_dline(winhpp[band]);
			winh_bsamp(winhpp[band]) +=
				wp->bsamp * winh_dsamp(winhpp[band]);
		}
	}
 /*
  * write window header
  */
	if (winhwrite(o_fd, winhpp) == ERROR) {
		error("can't write window header");
	}
	(void) winhfree(winhpp, nbands);
 /*
  * adjust and write geodetic headers, if present
  */
	if (geohpp != NULL) {
		for (band = 0; band < nbands; ++band) {
			if (geohpp[band] != NULL) {
				geoh_bline(geohpp[band]) +=
					wp->bline * geoh_dline(geohpp[band]);
				geoh_bsamp(geohpp[band]) +=
					wp->bsamp * geoh_dsamp(geohpp[band]);
			}
		}

		if (geohwrite(o_fd, geohpp) == ERROR) {
			error("can't write geodetic header");
		}
		(void) geohfree(geohpp, nbands);
	}
 /*
  * copy diverted headers
  */
	scr_fd = uropen(scr_name);
	if (scr_fd == ERROR) {
		error("can't reopen scratch file \"%s\" for reading",
		      scr_name);
	}

	copyhdrs(scr_fd, nbands, o_fd);

	if (boimage(o_fd) == ERROR) {
		error("can't terminate header output");
	}

	if (uremove(scr_name) == ERROR) {
		error("can't remove scratch file \"%s\"", scr_name);
	}
	SAFE_FREE(scr_name);
 /*
  * allocate I/O buffer
  */
	i_sampsize = sampsize(i_fd);
	i_linesize = i_nsamps * i_sampsize;

	buf = ecalloc(i_linesize, 1);
	if (buf == NULL) {
		error("can't allocate I/O buffer");
	}
 /*
  * set size of output line
  */
	o_linesize = wp->nsamps * i_sampsize;
 /*
  * skip to beginning of window
  */
	if (wp->bline > 0) {
		long            nskip;

		nskip = wp->bline;
		nskip *= i_linesize;

		if (urskip(i_fd, nskip) != nskip) {
			error("can't skip to start of window");
		}
	}
 /*
  * compute input line offsets
  */
	bskip = wp->bsamp;
	bskip *= i_sampsize;

	eskip = i_linesize - (bskip + o_linesize);
 /*
  * read lines; write portion of line within window
  */
	for (line = 0; line < wp->nlines; ++line) {
		if (bskip > 0) {
			if (urskip(i_fd, bskip) != bskip) {
				error("can't skip over start of line %d",
				      line + wp->bline);
			}
		}

		if (uread(i_fd, buf, o_linesize) != o_linesize) {
			error("image read error, line %d", line + wp->bline);
		}

		if (uwrite(o_fd, buf, o_linesize) != o_linesize) {
			error("image write error, line %d", line);
		}

		if (eskip > 0) {
			if (urskip(i_fd, eskip) != eskip) {
				error("can't skip over end of line %d",
				      line + wp->bline);
			}
		}
	}

	SAFE_FREE(buf);
 /*
  * done
  */
}
