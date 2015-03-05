#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 * scale -- scale an image by an arithmetic operator
 *
 * write output to scratch file, keeping track of min and max
 */

void
scale(
	char		aop,		/* operation		 */
	float		val)		/* value		 */
{
	REG_1 fpixel_t *op;		/* -> obuf		 */
	REG_2 fpixel_t *ip;		/* -> ibuf		 */
	REG_3 pixel_t  *mp;		/* -> mbuf		 */
	REG_4 int       samp;		/* pixel index in line	 */
	int             nsamps;		/* # pixels per line	 */
	int             nwrite;		/* # pixels written	 */
	fpixel_t       *ibuf;		/* -> input buffer	 */
	pixel_t        *mbuf;		/* -> mask buffer	 */
	fpixel_t       *minmax;		/* -> min/max each line	 */
	fpixel_t       *mm;		/* -> minmax		 */
	fpixel_t       *obuf;		/* -> output buffer	 */
	int             fdt;		/* temp file descriptor	 */
	int             line;		/* line index		 */
	int             nlines;		/* # lines in image	 */

	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	ibuf = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}
 /* NOSTRICT */
	if (parm.m_fd != ERROR) {
		mbuf = (pixel_t *) ecalloc(nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error("can't allocate mask buffer");
		}
	} else {
		mbuf = (pixel_t *) NULL;
	}
 /* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error("can't allocate output buffer");
	}
 /*
  * create temporary file
  */
	if ((parm.tmpnam = mktemplate("scale")) == NULL) {
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
		if (parm.m_fd != ERROR)
			pvread(parm.m_fd, mbuf, nsamps);
		nwrite = fpvread(parm.i_fd, ibuf, nsamps);
		if (nwrite < nsamps) {
			warn("premature end of image, line %d", line);
			if (nwrite <= 0)
				break;
		}
		ip = ibuf;
		mp = mbuf;
		op = obuf;
		for (samp = nwrite; --samp >= 0;) {
			if (parm.m_fd == ERROR || *mp) {
				switch (aop) {
					case OP1:
						*op = *ip + val;
						break;
					case OP2:
						*op = *ip - val;
						break;
					case OP3:
						*op = *ip * val;
						break;
					case OP4:
						if (val == 0.0) {
							error("scaling value cannot be 0.0 for divide operation");
						}
						*op = *ip / val;
						break;
					case OP5:
						if ((*ip == 0.0)&&(val < 0.0)) {
							error("exponent must be positive for images containing zero values");
						}
						if ( (*ip<0.0) && ( (val != (float) ((int)val) ) ) ) {
							error("exponent must be an integer for images containing negative values");
						}
							
						*op = pow(*ip,val);
						break;
					case OP6:
						if (*ip < 0.0) {
							error("image value must be positive for sqrt operation");
						}
						*op = sqrt(*ip);
						break;
					case OP7:
						if (*ip < 0.0) {
							error("image value must be positive for ln operation");
						}
						*op = log(*ip);
						break;
					case OP8:
						*op = exp(*ip);
						break;
					case OP9:
						if (*ip < 0.0) {
							error("image value must be positive for log operation");
						}
						*op = log10(*ip);
						break;
					case OP10:
						*op = pow(10.0,*ip);
						break;
				}
			} else {
				if (parm.got_no_data) {
					*op = parm.no_data;
				} else {
					*op = *ip;
				}
			}
			++op;
			++mp;
			++ip;
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
