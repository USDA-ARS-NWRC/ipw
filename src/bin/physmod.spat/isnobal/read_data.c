/*
 ** NAME
 ** 	read_data -- read all input data for next line from files
 **
 ** SYNOPSIS
 **	#include "pgm.h"
 **
 **	void
 **	read_data(
 **		int	first_step)	|* is this the first data tstep? *|
 **
 ** DESCRIPTION
 ** 	read_data reads all input data from the input, mask, precip,
 **	and initial conditions or temporary file for the next line.
 **
 ** GLOBAL VARIABLES READ
 ** 	fdi1
 ** 	fdi2
 ** 	fdic
 ** 	fdm
 ** 	fdp
 ** 	fdti
 **	line
 ** 	nsamps
 **
 ** GLOBAL VARIABLES MODIFIED
 ** 	ibuf1
 ** 	ibuf1_p
 ** 	ibuf2
 ** 	ibuf2_p
 ** 	icbuf
 ** 	icbuf_p
 ** 	itbuf
 ** 	itbuf_p
 ** 	mbuf
 ** 	mbuf_p
 ** 	pbuf
 ** 	pbuf_p
 */

#include <string.h>
#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"


void
read_data(
		int	first_step)	/* is this the first data tstep? */
{
//	int	t_nbytes;	/* #bytes in line of temp file	 */
	int N;			/* number of points */

	N = nsamps*nlines;

	/* read line of input images */

	if (first_step) {
		if (fpvread(fdi1, ibuf1, N) != N) {
			error("error reading first input image");
		}
	} else {
		memcpy(ibuf1, ibuf2, N * IBANDS * sizeof(fpixel_t));
	}

	if (fpvread(fdi2, ibuf2, N) != N) {
		error("error reading second input image");
	}

//	ibuf1_p = ibuf1;
//	ibuf2_p = ibuf2;

	/* read line of init-cond and mask images or temp results */

	if (first_step) {
		if (fdm != ERROR) {
			if (pvread(fdm, mbuf, N) != N) {
				error("error reading mask image");
			}
//			mbuf_p = mbuf;
		}
		if (fpvread (fdic, icbuf, N) != N) {
			error("error reading init-cond file");
		}
//		icbuf_p = icbuf;
	} else {
		// This works (I believe) because itbuf and otbuf will have the same address
		// But the _p will increment seperately, ensuring extract_data and assign_buffers
		// are working on the same pixel
		// itbuf = otbuf; itbuf_p = itbuf; will work but is not needed
//		itbuf_p = otbuf;
	}

	/* if precip data, read line of precip image */

	if (fdp != ERROR) {
		if (fpvread(fdp, pbuf, N) != N) {
			error("error reading precip image");
		}
//		pbuf_p = pbuf;
	}
}
