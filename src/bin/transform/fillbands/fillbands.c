#include "ipw.h"
#include "fpio.h"
#include "bih.h"

#include "pgm.h"

void
fillbands(
	int             fdi,		/* input file descriptor	 */
	int             fdo,		/* output file descriptor	 */
	int		outbands)	/* # bands to interpolate 	 */
{
	fpixel_t      **ibuf;		/* input buffers		 */
	fpixel_t      **obuf;		/* output buffer		 */
	fpixel_t        delta; 		/* span of input bands for 1 pix.*/ 
	int             i, j, k;	/* counters			 */
	int             nsamps;		/* # samples / line		 */
	int             nlines;		/* # lines			 */

 /*
  * Allocate I/O buffers of length nsamps. 
  */
	nsamps = hnsamps(fdi);
	nlines = hnlines(fdi);

	ibuf = (fpixel_t **) allocnd(sizeof(fpixel_t), 2, nsamps, IBANDS);
	if (ibuf == NULL) {
		error("Can't allocate input buffer");
	}

	obuf = (fpixel_t **) allocnd(sizeof(fpixel_t), 2, nsamps, outbands);
	if (ibuf == NULL) {
		error("Can't allocate output buffer");
	}

/* loop over lines */

	for (i = 0; i < nlines; i++) {

		/* read a line */

		if (fpvread(fdi, ibuf[0], nsamps) != nsamps)
			error("Read error, line %d", i);


		/* loop over samples */

		for (j = 0; j < nsamps; j++) {

		    delta = ibuf[j][1] - ibuf[j][0];

		    /* loop over bands */

		    for (k = 0; k < outbands; k++) {

			/* interpolate */

			obuf[j][k] = ibuf[j][0] + 
			    ( (float)(k+1) / (float)(outbands+1) * delta );

		    } /* end band loop */	
		}     /* end samp loop */	

/* fprintf(stderr, "fillbands: writing line %d \n", i); */

 		/* write output line */

		if (fpvwrite(fdo, obuf[0], nsamps) != nsamps) {
			error("Can't write line");
		}
	} /* end line loop */

}
