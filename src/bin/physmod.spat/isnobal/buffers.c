/*
** NAME
** 	buffers -- allocate I/O buffers for isnobal
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	buffers(void)
** 
** DESCRIPTION
** 	buffers allocates the I/O buffers for the the input images, initial
**	conditions image, precipitation image, mask image and temporary
**	results files.
** 
** GLOBAL VARIABLES READ 
**	fdm
**	nsamps
**	pfp
**
** GLOBAL VARIABLES MODIFIED 
**	ibuf1
**	ibuf2
**	icbuf
**	itbuf
**	mbuf
**	obuf
**	otbuf
**	pbuf
**	sbuf
*/

#include "ipw.h"
#include "pgm.h"

void
buffers(void)
{
	int		ic_nbands;	/* # bands in init-conds image   */
	int N;			/* number of points */

	N = nsamps*nlines;

   /* Allocate input buffers */

	ibuf1 = (fpixel_t *) ecalloc (N * IBANDS, sizeof(fpixel_t));
	if (ibuf1 == NULL) {
		error ("can't allocate first input buffer");
	}
	ibuf2 = (fpixel_t *) ecalloc (N * IBANDS, sizeof(fpixel_t));
	if (ibuf2 == NULL) {
		error ("can't allocate second input buffer");
	}

   /* Allocate init/restart buffer */

	if (restart) {
		ic_nbands = ICBANDS_RESTART;
	} else {
		ic_nbands = ICBANDS;
	}
	icbuf = (fpixel_t *) ecalloc (N * ic_nbands, sizeof(fpixel_t));
	if (icbuf == NULL) {
		error("can't allocate init/restart buffer");
	}

   /* Allocate precip buffer */

	if (pfp != NULL) {
		pbuf = (fpixel_t *) ecalloc (N * PBANDS, sizeof(fpixel_t));
		if (pbuf == NULL) {
			error("can't allocate precip buffer");
		}
	} else {
		pbuf = NULL;
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (N, sizeof(pixel_t));
		if (mbuf == NULL) {
			error("can't allocate mask buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate energy/mass output buffer */

	embuf = (fpixel_t *) ecalloc (N * EMBANDS, sizeof(fpixel_t));
	if (embuf == NULL) {
		error("can't allocate energy/mass output buffer");
	}

   /* Allocate snow output buffer */

	sbuf = (fpixel_t *) ecalloc (N * SBANDS, sizeof(fpixel_t));
	if (sbuf == NULL) {
		error("can't allocate snow output buffer");
	}

   /* Allocate temporary results buffers
    * Each sample requires TBANDS doubles + 1 double for mask value */

	itbuf = (double *) ecalloc (N * (TBANDS + 1), sizeof(double));
	if (itbuf == NULL) {
		error("can't allocate temporary results buffer");
	}
	otbuf = (double *) ecalloc (N * (TBANDS + 1), sizeof(double));
	if (otbuf == NULL) {
		error("can't allocate temporary results buffer");
	}
}
