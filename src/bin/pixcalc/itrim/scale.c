#include <math.h>

#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"

#include "pgm.h"

/*
 * scale -- this is the meat of the itrim program
 *
 * write output to scratch file, keeping track of min and max
 */

void
scale(
	float   min,			/* output min/max values */
	float   max,			/* output min/max values */
        float   nodatamin,              /* nodata val for vals < min */
        float   nodatamax)              /* nodata val for vals > max */
{
	REG_1 fpixel_t *op;		/* -> obuf		 */
	REG_2 fpixel_t *ip;		/* -> ibuf		 */
	REG_4 int       samp;		/* pixel index in line	 */
	int       	band;		/* band index in line	 */
	int             nsamps;		/* # pixels per line	 */
	int             nwrite;		/* # pixels written	 */
	fpixel_t       *ibuf;		/* -> input buffer	 */
	fpixel_t       *minmax;		/* -> min/max each line	 */
	fpixel_t       *mm;		/* -> minmax		 */
	fpixel_t       *bandmm;		/* -> minmax for 1 band  */
	fpixel_t       *obuf;		/* -> output buffer	 */
	int             fdt;		/* temp file descriptor	 */
	int             line;		/* line index		 */
	int             nlines;		/* # lines in image	 */
	int		i;

	nlines = hnlines(parm.i_fd);
	nsamps = hnsamps(parm.i_fd);
 /*
  * allocate I/O buffers
  */
 /* NOSTRICT */
	ibuf = (fpixel_t *) ecalloc(nsamps * parm.ibands, sizeof(fpixel_t));
	if (ibuf == NULL) {
		error("can't allocate input buffer");
	}
 /* NOSTRICT */
	obuf = (fpixel_t *) ecalloc(nsamps * parm.obands, sizeof(fpixel_t));
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
	minmax = (fpixel_t *) ecalloc(nlines * parm.obands * 2, 
		sizeof(fpixel_t));
	if (minmax == NULL) {
		error("can't allocate minmax");
	}
	mm = minmax;

	bandmm = (fpixel_t *) ecalloc(nlines * 2, sizeof(fpixel_t));
	if (bandmm == NULL) {
		error("can't allocate bandmm");
	}

 /*
  * read input and create temporary output image; calculate min and max
  * of each line as we go along
  */

	for (line = 0; line < nlines; line++) {
		nwrite = fpvread(parm.i_fd, ibuf, nsamps);
		if (nwrite < nsamps) {
			error("premature end of image, line %d", line);
			if (nwrite <= 0) break;
		}

		ip = ibuf;
		op = obuf;

		for (samp = 0; samp < nsamps; samp++) {
		    for (band = 0; band < parm.obands; band++) {

			/* process pixels */

			if     (*ip < min) *op = nodatamin;
			else if(*ip > max) *op = nodatamax;
			else if(parm.bin)  *op = 1.0;
			else               *op = *ip;

			/*** fprintf(stderr,"in = %g  out = %g \n",*ip,*op);**/

			op++;
			ip++;
		    } /* end band loop */
		}     /* end samp loop */
 /*
  * output line - also calculate min and max
  */
		nwrite *= (sizeof(fpixel_t) * parm.obands);
 /* NOSTRICT */
		if (uwrite(fdt, (addr_t) obuf, nwrite) != nwrite) {
			error("temp file write error, line %d", line);
		}
		nwrite /= (sizeof(fpixel_t) * parm.obands);
		if (mnxfp(obuf, nwrite, parm.obands, mm) == ERROR) {
			error("can't calculate min/max, line %d", line);
		}

		mm += (parm.obands * 2);

	} /* end line loop */
 /*
  * calculate min/max for each band
  */
	if (line < nlines) {	
		error("premature end of input image");
	}

        /* extract one band's min/max values for all lines */
  
 	for (band = 0; band < parm.obands; band++) {
	    i = 2 * band;
 	    for (line = 0; line < nlines * 2; line+=2) {
	        bandmm[line]   = minmax[i]; 
	        bandmm[line+1] = minmax[i + 1]; 
		i += (parm.obands * 2);
	    }

 	 /* find min/max for this band, store in parm */

	    if (mnxfp(bandmm, 2 * nlines , 1, &(parm.mmval[band][0])
		) == ERROR) {
			error("can't calculate global min/max");
	    }
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
 /* NOSTRICT */
	free((addr_t) bandmm);
}
