/*
** NAME
** 	gtoporad -- orchestrate computations for the gtoporad program
** 
** SYNOPSIS
**	void gtoporad (fdi, fdo, net, nbits)
**	int fdi, fdo;
**	bool_t net;
**	int nbits;
** 
** DESCRIPTION
**
**	This routine orchestrates computations for the toporad
**	program.
**
**	1.  Read headers and allocate buffers.
**	2.  Read input data and pass them to 'radcalc' routine.
**	3.  Write output data to temporary file, keeping track of
**	    minimum and maximum radiation values.
**	4.  Write output header, now that minimum and maximum are
**	    known.
**	5.  Read data from temporary file and write to output file.
** 
*/

#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

void
gtoporad (
	int             fdi,		/* input file descriptor	 */
	int             fdo,		/* output file descriptor	 */
	bool_t          net,		/* ? net radiation		 */
	int		nbits)		/* # output bits		 */
{
	char           *tname;		/* temporary file name		 */
	fpixel_t        rval[2];	/* min, max output values	 */
	fpixel_t       *in_buf;		/* input buffer			 */
	fpixel_t       *mmhold;		/* min, max of all lines	 */
	fpixel_t       *out_buf;	/* output buffer		 */
	fpixel_t       *pmmhold;	/* -> mmhold			 */
	int             fdt;		/* temporary file descriptor	 */
	int             line;		/* line index			 */
	int             linesgot;	/* # lines read			 */
	int             ngot;		/* # pixels read		 */
	int             nlines;		/* # lines in image		 */
	int             nsamps;		/* # pixels in line		 */


   /* Read input headers and write some of the output headers. */

	if (head_init (fdi, fdo, nbits) == ERROR) {
		error ("gtoporad,reading headers");
	}

   /* number of lines and number of pixels in an input line */

	nlines = hnlines (fdi);
	nsamps = hnsamps (fdi);

   /* allocate buffers: input and output for each line */

	in_buf = (fpixel_t *) ecalloc (nsamps * NBANDS, sizeof(fpixel_t));
	if (in_buf == NULL) {
		error ("can't allocate input buffer");
	}

	out_buf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (out_buf == NULL) {
		error ("can't allocate output buffer");
	}

   /* allocate buffer to hold min & max for each line */

	mmhold = (fpixel_t *) ecalloc (nlines * 2, sizeof(fpixel_t));
	if (mmhold == NULL) {
		error ("can't allocate buffer to hold min/max values");
	}
	pmmhold = mmhold;

   /* open temporary file to store radiation values */

	tname = mktemplate("topo");
	if (tname == NULL) {
		error ("can't allocate name of temporary file");
	}
	fdt = uwopen_temp(tname);
	if (fdt == ERROR) {
		error ("can't create temporary file");
	}

   /* read each line of image, calculate radiation values, and store */

	for (line = 0, linesgot = 0; line < nlines; ++line) {
		if ((ngot = fpvread (fdi, in_buf, nsamps)) != nsamps) {
			if (ngot < 0) {
				warn ("read error, line %d", line);
				break;
			} else {
				warn ("line %d: # pixels = %d; should be %d",
				     line, ngot, nsamps);
			}
		}

   		/* radcalc routine calculates radiation values */

		if (radcalc (in_buf, ngot, net, out_buf) == ERROR) {
			error ("gtoporad, calculating radiation");
		}

		/* Find minimum and maximum of this line; advance pointer */
		/* to get ready for next line.  */

		if (mnxfp (out_buf, ngot, 1, pmmhold) == ERROR) {
			error ("toporad, from mnxfp");
		}
		pmmhold += 2;

		/* write this line to temporary storage file */

		if (uwrite (fdt, (addr_t) out_buf, ngot * sizeof(fpixel_t))
			== ERROR) {
			error ("toporad, writing to temporary file");
		}
		++linesgot;
	}
	if (linesgot != nlines) {
		error ("lines read = %d, should be %d", linesgot, nlines);
	}

   /* global minimum and maximum values */

	if (mnxfp (mmhold, nlines * 2, 1, rval) == ERROR) {
		error("toporad, from mnxfp");
	}

   /* Finish writing headers, including LQH.  */

	if (head_final (fdo, rval) == ERROR) {
		error ("toporad: writing final headers");
	}

   /* Read floating point values from temporary file and write to output */
   /* file, converting to LQH as we go. */

	(void) uclose (fdt);
	fdt = uropen (tname);
	if (fdt == ERROR) {
		error ("gtoporad: can't re-open temporary file");
	}
	for (line = 0; line < nlines; ++line) {
		if (uread (fdt, (addr_t) out_buf, nsamps * sizeof(fpixel_t))
		    == ERROR) {
			error ("gtoporad, reading from temporary file");
		}
		if (fpvwrite (fdo, out_buf, nsamps) == ERROR) {
			error ("gtoporad, write error on output file");
		}
	}

   /* get rid of temporary file and free buffers */

	(void) uclose (fdt);
	(void) unlink (tname);
	free ((addr_t) in_buf);
	free ((addr_t) out_buf);
	free ((addr_t) mmhold);
}
