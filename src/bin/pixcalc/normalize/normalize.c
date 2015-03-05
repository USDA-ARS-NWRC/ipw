#include "ipw.h"
#include "bih.h"

#include "pgm.h"

/*
 * normalize - ensure that daily precip proportions sum to 1.0.
 *
 * write output to scratch file, keeping track of min and max
 */

void
norm(void)
{
    int             band;       /* band index            */
    int             line;       /* line index            */
    int             samp;       /* pixel index in line   */
    int             nwrite;     /* # pixels written      */
    int             nread;      /* # pixels read         */
    fpixel_t       *ibuf;       /* -> input buffer       */
    int             prcp_index; /* # of non-zero days    */
    double	    prcp_sum;   /* month precip sum      */
    double 	    delta;      /* scaling value         */
    int 	    fdt;	/* temp file descriptor  */

    nwrite = parm.nbands * parm.nsamps * sizeof(fpixel_t);
 /*
  * allocate input buffer
  */

    ibuf = (fpixel_t *) ecalloc(parm.nbands * parm.nsamps, sizeof(fpixel_t));
    if (ibuf == NULL) {
        error("can't allocate input buffer");
    }

 /*
  * create temporary file
  */
        if ((parm.tmpnam = mktemplate("normalize")) == NULL) {
                error("can't create temporary file name");
        }
        if ((fdt = uwopen_temp(parm.tmpnam)) == ERROR) { 
                error("can't create temporary file");
        }

/*
 * read row
 */
    for (line = 0; line < parm.nlines; line++)
      {
          nread = fpvread (parm.i_fd, ibuf, parm.nsamps);
          if (nread != parm.nsamps)
            {
                warn("premature end of image, line %d", line);
            }

/*** debug dump 
	  fprintf(stderr, "*** line %d *** \n", line);
          for (samp = 0; samp < parm.nsamps; samp++) {
            for (band = 0; band < parm.nbands; band++){
		fprintf(stderr, "%6.3f", ibuf[samp*parm.nbands + band] );
	    }
	    fprintf(stderr, "\n");
	  }
***/
	
/*
 * loop through samples
 */
          for (samp = 0; samp < parm.nsamps; samp++)
          {
                prcp_index = 0;
                prcp_sum = 0.0;

/*
 * loop through the bands, counting non-zeros and adding values
 */
                for (band = 0; band < parm.nbands; band++)
                {

                      if (ibuf[samp*parm.nbands + band] > 0.0)
                      {
                            prcp_sum += ibuf[samp*parm.nbands + band];
                            prcp_index++;
                      }
                }

/*
 * If there was no precipitation for the month take care of it now.
 */

                if (prcp_index == 0)
                {
                    prcp_sum = 1.0;
                }

/* 
 * If sum is off, scale it properly.  Make sure none go negative. 
 */

                if (ABS(prcp_sum - 1.0) > 0.000001)
                {
                      delta = 1.0 / prcp_sum;   

                      for (band = 0; band < parm.nbands; band++)
                      {
			  if (ibuf[samp*parm.nbands + band] > 0.0) 
			  {
                              ibuf[samp*parm.nbands + band] *= delta;
                              if (ibuf[samp*parm.nbands + band] < 0.0)
                              {
                                  ibuf[samp*parm.nbands + band] = 0.0;
                              }
/*** debug dump
 fprintf(stderr,"old pix: %f; new pix: %f\n",
    ibuf[samp*parm.nbands + band],ibuf[samp*parm.nbands + band] * delta);
*/
			  } /* end if */
                      }     /* end band loop */
                  }         /* end if */
            }               /* end sample loop */
    
/*
 * Write out the line to the output file
 */


          if ((uwrite (fdt, ibuf, nwrite)) != nwrite)
          {
                warn("Write to output file failed");
          }
      
      } /* end line loop */

 /*
  * clean up
  */
    free((addr_t) ibuf);
    (void) uclose (fdt);

}

