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

#include "ipw.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
read_data(
	int	first_step)	/* is this the first data tstep? */
{
	int	t_nbytes;	/* #bytes in line of temp file	 */

   /* read line of input images */

	if (fpvread(fdi1, ibuf1, nsamps) != nsamps) {
		error("error reading first input image, line %d", line);
	}
	if (fpvread(fdi2, ibuf2, nsamps) != nsamps) {
		error("error reading second input image, line %d", line);
	}

	ibuf1_p = ibuf1;
	ibuf2_p = ibuf2;

   /* read line of init-cond and mask images or temp results */

	if (first_step) {
		if (fdm != ERROR) {
			if (pvread(fdm, mbuf, nsamps) != nsamps) {
				error("error reading mask image, line %d", line);
			}
			mbuf_p = mbuf;
		}
		if (fpvread (fdic, icbuf, nsamps) != nsamps) {
			error("error reading init-cond file, line %d", line);
		}
		icbuf_p = icbuf;
	} else {
		/* read length of temporary file line */

		if (uread(fdti, (addr_t) &t_nbytes, sizeof(int))
				!= sizeof(int)) {
			error("error reading temp file, line %d", line);
		}

		/* check length of line - should be between nsamps (all masked)
		   and nsamps * (TBANDS + 1) (none masked) */

		if (t_nbytes < nsamps * sizeof(double) ||
		    t_nbytes > nsamps * (TBANDS +1) * sizeof(double)) {
			error("Illegal temporary file line length: %d", t_nbytes);
		}

		/* read temporary file line */

		if (uread(fdti, (addr_t) itbuf, t_nbytes) != t_nbytes) {
			error("error reading temp file, line %d", line);
		}

		itbuf_p = itbuf;
	}

   /* if precip data, read line of precip image */

	if (fdp != ERROR) {
		if (fpvread(fdp, pbuf, nsamps) != nsamps) {
			error("error reading precip image, line %d", line);
		}
		pbuf_p = pbuf;
	}
}
