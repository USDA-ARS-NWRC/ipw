#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 * deltaT -- calculate diurnal temperature range from Tmax (band0),
 * Tmin1 (band1), and Tmin2 (band 2)
 *
 */

void
deltaT(void)
{
	REG_1 fpixel_t *op;		/* -> obuf		 */
	REG_2 fpixel_t *ip;		/* -> ibuf		 */
	REG_4 int       nbands;		/* # bands		 */
	REG_5 int       samp;		/* pixel index in line	 */
	int             nsamps;		/* # pixels per line	 */
	int             nwrite;		/* # pixels written	 */
	fpixel_t       *ibuf;		/* -> input buffer	 */
	fpixel_t       *minmax;		/* -> min/max each line	 */
	fpixel_t       *mm;		/* -> minmax		 */
	fpixel_t       *obuf;		/* -> output buffer	 */
	fpixel_t        b1;		/* Tmax band		 */
	fpixel_t        b2;		/* Tmin1 band		 */
	fpixel_t        b3;		/* Tmin2 band		 */
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
	if ((parm.tmpnam = mktemplate("deltaT")) == NULL) {
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
  * read input and create temporary delta T output image; calculate min and max
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
			b1 = *ip;
			b2 = *(ip+1);
			b3 = *(ip+2);
			*op = b1 - (b2 + b3) / 2.0;
			ip += nbands;
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
  * calculate global min/max, to set output lqh
  */
	if (line < nlines) {		/* allow for premature end of
					 * image */
		nlines = line;
	}
	if (mnxfp(minmax, 2 * nlines, 1, &(parm.mmval[0])) == ERROR) {
		error("can't calculate global min/max");
	}
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
