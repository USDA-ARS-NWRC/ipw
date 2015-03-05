#include "ipw.h"
#include "bih.h"

#include "pgm.h"

/*
 * iadd -- add (or subtract) bands
 *
 * write output to scratch file, keeping track of min and max
 */

void
iadd()
{
	REG_1 fpixel_t *op;		/* -> obuf		 */
	REG_2 fpixel_t *ip;		/* -> ibuf		 */
	REG_3 int       band;		/* band index		 */
	REG_4 int       nbands;		/* # bands		 */
	REG_5 int       samp;		/* pixel index in line	 */
	int             nsamps;		/* # pixels per line	 */
	int             nwrite;		/* # pixels written	 */
	fpixel_t       *ibuf;		/* -> input buffer	 */
	fpixel_t       *minmax;		/* -> min/max each line	 */
	fpixel_t       *mm;		/* -> minmax		 */
	fpixel_t       *obuf;		/* -> output buffer	 */
	int             fdt;		/* temp file descriptor	 */
	int             line;		/* line index		 */
	int             nlines;		/* # lines in image	 */

	nbands = hnbands(parm.i_fd);
	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	ibuf = (fpixel_t *) ecalloc(nbands * nsamps, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}
 /* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * create temporary file
  */
	if ((parm.tmpnam = mktemplate("iadd")) == NULL) {
		error("can't create temporary file name");
	}
	if ((fdt = uwopen_temp(parm.tmpnam)) == ERROR) {
		error("can't create temporary file");
	}
 /*
  * vector to hold min and max of each line
  */
 /* NOSTRICT */
	mm = minmax = (fpixel_t *) ecalloc(nlines * 2, sizeof(fpixel_t));
	if (minmax == NULL) {
		error("can't store min and max of each line");
	}
 /*
  * read input and create temporary output image; calculate min and max
  * of each line as we go along
  */
	for (line = 0; line < nlines; ++line) {
		nwrite = fpvread(parm.i_fd, ibuf, nsamps);
		if (nwrite < nsamps) {
			warn("premature end of image, line %d", line);
			if (nwrite <= 0)
				break;
		}
		ip = ibuf;
		op = obuf;
		for (samp = nwrite; --samp >= 0;) {
			*op = 0;
			for (band = nbands; --band >= 0;) {
				*op += *ip++;
			}
			++op;
		}
 /*
  * output line - also calculate min and max
  */
		nwrite *= sizeof(fpixel_t);
 /* NOSTRICT */
		if (uwrite(fdt, (addr_t) obuf, nwrite) != nwrite) {
			error("temp file write error, line %d", line);
		}
		nwrite /= sizeof(fpixel_t);
		if (mnxfp(obuf, nwrite, 1, mm) == ERROR) {
			error("can't calculate min/max, line %d", line);
		}
		mm += 2;
	}
 /*
  * calculate global min/max
  */
	if (line < nlines) {		/* allow for premature end of
					 * image */
		nlines = line;
	}
	if (mnxfp(minmax, 2 * nlines, 1, &(parm.mmval[0])) == ERROR) {
		error("can't calculate global min/max");
	}
	if (parm.mmval[0] == parm.mmval[1])
	  parm.mmval[1] += 1;
 /*
  * clean up
  */
	(void) uclose(fdt);
 /* NOSTRICT */
	free((addr_t) ibuf);
 /* NOSTRICT */
	free((addr_t) obuf);
 /* NOSTRICT */
	free((addr_t) minmax);
}
