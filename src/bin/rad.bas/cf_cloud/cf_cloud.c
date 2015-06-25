/*
 **	This routine orchestrates computations for the cloud
 **	program.
 **
 **	1.  Read headers and allocate buffers.
 **	2.  Read input data and pass them to 'cloudcalc' routine.
 **	3.  Write output data to temporary file, keeping track of
 **	    minimum and maximum radiation values.
 **	4.  Write output header, now that minimum and maximum are
 **	    known.
 **	5.  Read data from temporary file and write to output file.
 */

#include "ipw.h"
#include "bih.h"
#include "fpio.h"

#include "pgm.h"

void
cf_cloud(
		int             fdi,		/* input file descriptor	 */
		int             fdo)		/* output file descriptor	 */
{
	char           *tname;		/* temporary file name		 */
	int		obands;		/* # of output bands = 3	 */
	fpixel_t       *in_buf;		/* input buffer			 */
	fpixel_t       *out_buf;	/* output buffer		 */
	fpixel_t        line_mnx[4];	/* min, max output values: line  */
	fpixel_t        min_max[4];	/* min, max output values: image */
	int		i;		/* loop counter			 */
	int             fdt;		/* temporary file descriptor	 */
	int             line;		/* line index			 */
	int             linesgot;	/* # lines read			 */
	int             ngot;		/* # pixels read		 */
	int             nlines;		/* # lines in image		 */
	int             nsamps;		/* # pixels in line		 */


	/*
	 * Read input headers and write some of the output headers.
	 */
	obands = OBANDS;
	if (head_init(fdi, fdo, obands) == ERROR) {
		error("cf_cloud, reading headers");
	}
	/*
	 * number of lines and number of pixels in an input line
	 */
	nlines = hnlines(fdi);
	nsamps = hnsamps(fdi);
	/*
	 * allocate buffers: input and output for each line
	 */
	/* NOSTRICT */
	in_buf = (fpixel_t *) ecalloc(nsamps * NBANDS, sizeof(fpixel_t));
	if (in_buf == NULL) {
		error("can't allocate input buffer");
	}
	/* NOSTRICT */
	out_buf = (fpixel_t *) ecalloc(nsamps * obands, sizeof(fpixel_t));
	if (out_buf == NULL) {
		error("can't allocate output buffer");
	}
	/*
	 * open temporary file to store radiation values
	 */
	tname = mktemplate("cloud");
	if (tname == NULL) {
		error("can't allocate name of temporary file");
	}
	fdt = uwopen_temp(tname);
	if (fdt == ERROR) {
		error("can't create temporary file");
	}
	/*
	 * read each line of image, calculate radiation values, and store
	 */
	for (line = 0, linesgot = 0; line < nlines; ++line) {
		if ((ngot = fpvread(fdi, in_buf, nsamps)) != nsamps) {
			if (ngot < 0) {
				warn("read error, line %d", line);
				break;
			}
			else {
				warn("line %d: # pixels = %d; should be %d",
						line, ngot, nsamps);
			}
		}
		/*
		 * cloudcalc routine calculates corrected radiation values
		 */
		if (cloudcalc(in_buf, ngot, out_buf, obands) == ERROR) {
			error("cf_cloud, calculating corrected radiation");
		}
		/*
		 * Find minimum and maximum of this line; and compare to image minimum
		 * and maximums.
		 */
		if (mnxfp(out_buf, ngot, obands, line_mnx) == ERROR) {
			error("cf_cloud, from mnxfp");
		}
		if (line == 0) {
			/*
			 * first line's min and max are image's min & max
			 */
			for (i = 0; i < 2 * obands; i++)
				min_max[i] = line_mnx[i];
		}
		else  {
			for (i = 0; i < 2 * obands; i++) {
				if (i % 2 == 0) {
					/* minimum for a band */
					if (line_mnx[i] < min_max[i])
						min_max[i] = line_mnx[i];
				} else {
					/* maximum for a band */
					if (line_mnx[i] > min_max[i])
						min_max[i] = line_mnx[i];
				}
			}
		}
		/*
		 * write this line to temporary storage file
		 */
		if (uwrite(fdt, (addr_t) out_buf,
				ngot * obands * sizeof(fpixel_t)) == ERROR) {
			error("cf_cloud, writing to temporary file");
		}
		++linesgot;
	}
	if (linesgot != nlines) {
		error("lines read = %d, should be %d", linesgot, nlines);
	}
	/*
	 * Finish writing headers, including LQH.
	 */
	if (head_final(fdo, min_max, obands) == ERROR) {
		error("cf_cloud: writing final headers");
	}
	/*
	 * Read floating point values from temporary file and write to output
	 * file, converting to LQH as we go.
	 */
	(void) uclose(fdt);
	fdt = uropen(tname);
	if (fdt == ERROR) {
		error("cf_cloud: can't re-open temporary file");
	}
	for (line = 0; line < nlines; ++line) {
		if (uread(fdt, (addr_t) out_buf,
				nsamps * obands * sizeof(fpixel_t)) == ERROR) {
			error("cf_cloud, reading from temporary file");
		}
		if (fpvwrite(fdo, out_buf, nsamps) == ERROR) {
			error("cf_cloud, write error on output file");
		}
	}
	/*
	 * get rid of temporary file and free buffers
	 */
	(void) uclose(fdt);
	(void) unlink(tname);
	free((addr_t) in_buf);
	free((addr_t) out_buf);
}
