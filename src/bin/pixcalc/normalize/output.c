#include "ipw.h"
#include "fpio.h"
#include "hdrio.h"
#include "lqh.h"

#include "pgm.h"

/*
 * output -- read temporary file, get min/max for each band,
 * write lq headers, and write output file
 */

void
output(void)
{
	fpixel_t       *buf, *bufp;	/* I/O buffer and pointer       */
	int             fdt;		/* temporary file descriptor	*/
	int             npix;		/* # pixels to write		*/
	int             nbytes;		/* # bytes to read		*/
	int	 	line,samp,band; /* loop indices			*/
	LQH_T	      **o_lqh;		/* output lq header		*/
	pixel_t		ibkpt[2];	/* breakpoints for lq header    */
	fpixel_t	fbkpt[2];
	fpixel_t       *min, *max;	/* arrays of min/max for each band */
	fpixel_t	val;		/* current pixel value		*/

/*
 * open temporary file
 */
	if ((fdt = uropen(parm.tmpnam)) == ERROR) {
		error("can't re-open temporary file");
	}
 /*
  * allocate I/O buffer
  */
	npix = parm.nbands * parm.nsamps;

	buf = (fpixel_t *) ecalloc(npix, sizeof(fpixel_t));
	if (buf == NULL) {
		error("can't create I/O buffer");
	}
	bufp = buf;
	nbytes = npix * sizeof(fpixel_t);

 /*
  * run through the temp file and find min/max for each band
  */

/*********** begin inserted code *************/

    /* allocate lq header */

    o_lqh = (LQH_T **) hdralloc(parm.nbands, sizeof(LQH_T *), 
	parm.o_fd, LQH_HNAME);
    assert(o_lqh != NULL);

    ibkpt[0] = 0;
    ibkpt[1] = ipow2(parm.nbits) - 1;

    /* compute min/max for each band */

    /* allocate min/max buffers */
    min = (fpixel_t *) ecalloc( npix, sizeof(fpixel_t));
    max = (fpixel_t *) ecalloc( npix, sizeof(fpixel_t));

    /* initialize min/max for each band to extreme values */
    for (band = 0; band < parm.nbands; band++) {
      min[band] = FLT_MAX;
      max[band] = (FLT_MAX - 1) * -1.0;  
      bufp++;
    }

    /* read a line, update min/max, continue for entire image file */
    for (line = 0; line < parm.nlines; line++) {
      bufp = buf; 
      if ( uread( fdt, (addr_t) bufp, nbytes) != nbytes) {
fprintf(stderr,"nbytes = %d\n",nbytes);
        error( "problems reading temporary file on line %d", line );
      }
      for (samp = 0; samp < parm.nsamps; samp++) {  
        for (band = 0; band < parm.nbands; band++) {
          val = *bufp++;
          if (val < min[band])  min[band] = val;
          if (val > max[band])  max[band] = val;
	}
      }
    }

    /* now that min/max are known, make and write the lq header */
    for (band = 0; band < parm.nbands; band++) {
      if ( min[band] == max[band] ) /* min cannot equal max in ipw */
        max[band]++;
      if (max[band] < min[band]) { /*&&& remove this? */
        fprintf(stderr, "NOTE: impossible situation?");
	min[band] = 0.0;
        max[band] = 1.0;
      }
      fbkpt[0] = (fpixel_t) min[band];
      fbkpt[1] = (fpixel_t) max[band];
      o_lqh[band] = lqhmake (parm.nbits, 2, ibkpt, fbkpt,
	(char *) NULL, LQH_LIN_INTERP);
    }
    (void) free (min);
    (void) free (max);

    if (lqhwrite(parm.o_fd, o_lqh) == ERROR) {
      error("couldn't write lq header");
    }

    /* terminate header output */
    if (boimage(parm.o_fd) == ERROR) {
      error("can't terminate header output");
    }

    /* now, read temp image and write ipw image, a line at a time */
    lseek ( fdt, 0L, 0 );
    for (line = 0; line < parm.nlines; line++) {
      bufp = buf; 
      if ( uread( fdt, (addr_t) bufp, nbytes) != nbytes) {
        error( "problems reading temporary file on line %d", line );
      }
      if ( fpvwrite( parm.o_fd, bufp, parm.nsamps) != parm.nsamps) {
        error( "problems writing IPW file on line %d", line );
      }
    }

/*********** end inserted code *************/

 /*
  * clean up
  */
	(void) uclose(fdt);
	(void) uremove(parm.tmpnam);
}
