#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

#define C 2.4 /* C coef (Bristow & Campbell, 1984) */

/*
 * tcloud -- calculate cloudiness attenuation coef. on solar radiation from
 * deltaT (band0), and (monthly) average deltaT (band1)
 * (see Bristow & Campbell, 1984)
 *
 */

void
tcloud(void)
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
	fpixel_t        b1;		/* deltaT band		 */
	fpixel_t        b2;		/* mean deltaT band	 */
	int             fdt;		/* temp file descriptor	 */
	int             line;		/* line index		 */
	int             nlines;		/* # lines in image	 */
	double		B;		/* B coef (Bristow & Campbell, 1984) */
	

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
	if ((parm.tmpnam = mktemplate("tcloud")) == NULL) {
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
  * read input and create temporary cloud attenuation output image;
  * calculate min and max of each line as we go along
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
			/*
			 ***	from Bristow and Campbell (1984), eq. 5
			 */
			B = 0.036 * exp ( -0.154 * b2 );
			/*
			 ***	from Bristow and Campbell (1984), eq. 4
			 */
			if (b1 < 0.0)
				b1 = 0.0;
			*op = 1 - exp (-1.0 * B * pow(b1, C));
			/* printf ("%f - %f : %f\n", b1, b2, *op); */

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
  * calculate global min/max, and check to insure range >= 0.0 && <= 1.0
  */
	if (line < nlines) {		/* allow for premature end of
					 * image */
		nlines = line;
	}
	if (mnxfp(minmax, 2 * nlines, 1, &(parm.mmval[0])) == ERROR) {
		error("can't calculate global min/max");
	}
 	if ((parm.mmval[0] < 0.0) || (parm.mmval[1] > 1.0)) {
 		error("global min/max outside the range of 0.0 to 1.0\nparm.mmval[0]=%g; parm.mmval[1]=%g", parm.mmval[0], parm.mmval[1]);
 	}
 /*
  * set min to 0.0 and max 1.0
  */
 	parm.mmval[0] = 0.0;
	parm.mmval[1] = 1.0;
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
