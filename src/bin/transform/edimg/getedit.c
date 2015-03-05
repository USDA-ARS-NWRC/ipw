#include "ipw.h"

#include "pgm.h"

/*
 * getedit -- get next image edit coordinates
 */

void
getedit(
	int             c_fd,		/* coordinate file descriptor	 */
	int             nlines,		/* # image lines		 */
	int             nsamps,		/* # samples / line		 */
	double          k,		/* default replacement val	 */
	int            *eline,		/* next line to edit		 */
	int            *esamp,		/* next sample to edit		 */
	double         *repl)		/* replacement value		 */
{
	static double   old_line = 0;	/* previous edit line		 */

	char            coords[MAX_INPUT];	/* input coord string	 */
	double          line;		/* edit line			 */
	int             ncoords;	/* # coords on input line	 */
	double          samp;		/* edit sample			 */

 /*
  * read line from coordinate file
  */
	if (ugets(c_fd, coords, sizeof(coords)) == NULL) {
 /*
  * if EOF then return with un-matchable edit line coord
  */
		if (ueof(c_fd)) {
			*eline = (-1);
			return;
		}

		error("coordinate read failed");
	}
 /*
  * extract edit coordinates from input line
  */
	ncoords = sscanf(coords, "%lf %lf %lf", &line, &samp, repl);
	if (ncoords == 2) {
		*repl = k;
	}
	else if (ncoords != 3) {
		error("invalid coordinates: %s", coords);
	}
 /*
  * make sure coordinates are sorted by line
  */
	if (line < old_line) {
		error("unsorted coordinates: %g,%g", line, samp);
	}

	old_line = line;
 /*
  * NB: would do coord transform here
  */
	*eline = line;
	*esamp = samp;
 /*
  * make sure coordinates are on the image
  */
	if (*eline < 0 || *eline >= nlines || *esamp < 0 || *esamp >= nsamps) {
		error("bad coordinates (not on image): %g,%g", line, samp);
	}
}
