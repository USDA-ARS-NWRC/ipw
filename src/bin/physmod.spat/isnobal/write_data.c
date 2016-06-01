/*
** NAME
** 	write_data -- write line buffers to all output files
** 
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	write_data(
**	    int	        output,		|* writing output image this step? *|
**	    int	        last_step)	|* last step?			   *|
** 
** DESCRIPTION
** 	write_data writes the line buffers to the temporary results file, the
**	temporary energy/mass output file and the temporary snow output file
**	(at the appropriate time steps).
** 
** GLOBAL VARIABLES READ
**	embuf
**	fdem
**	fds
**	fdto
**	line
**	nsamps
**	ot_nbytes
**	otbuf
**	sbuf
**
** GLOBAL VARIABLES MODIFIED
**
*/

#include "ipw.h"
#include "pgm.h"

static bool_t	first_call = TRUE;	/* first call to this function?    */
static int	em_nbytes;		/* # of bytes in e/m out line buff */
static int	s_nbytes;		/* # of bytes in snow out line buff*/

void
write_data(
	int	        output,		/* writing output image this step? */
	int	        last_step)	/* last step?			   */
{

	if (first_call) {
		em_nbytes = nsamps * nlines * EMBANDS * sizeof(fpixel_t);
		s_nbytes = nsamps * nlines * SBANDS * sizeof(fpixel_t);
		first_call = FALSE;
	}

   /* if not last step, then write temp results */

	if (! last_step) {
		/* write length of line to temp file */

//		if (uwrite(fdto, (addr_t) &ot_nbytes, sizeof(int))
//			 				!= sizeof(int)) {
//			error("temp file write error, line %d", line);
//		}
//
//		if (uwrite(fdto, (addr_t) otbuf, ot_nbytes) != ot_nbytes) {
//			error("temp file write error, line %d", line);
//		}
	}

   /* if output this time step, write to output files */

	if (output) {
		if (uwrite(fdem, (addr_t) embuf, em_nbytes) != em_nbytes) {
			error("energy/mass output file write error");
		}

		if (uwrite(fds, (addr_t) sbuf, s_nbytes) != s_nbytes) {
			error("snow output file write error");
		}
	}
}
