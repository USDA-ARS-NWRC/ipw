#include "ipw.h"

#include "pgm.h"

/*
 * getcoords -- read next pair of coordinates
 */

#define	LINELEN		MAX_INPUT

int
getcoords(
		int             fd,		/* coordinate file descriptor	 */
		int             nlines,		/* # image lines		 */
		int             nsamps,		/* # samples / line		 */
		int            *linep,		/* -> line coordinate		 */
		int            *sampp)		/* -> sample coordinate		 */
{
	static char    *buf = NULL;	/* coordinate input buffer	 */
	static double   old_line = 0;   /* previous line coordinate	 */
	double          line;		/* current line coordinate	 */
	double          samp;		/* current sample coordinate	 */
	double		xoffset;	/* for converting geo to line/samp */
	double		yoffset;	
	double		pt_x, pt_y;     /* geo input coords		 */

	/*
	 * if first time then allocate input buffer
	 */
	if (buf == NULL) {
		buf = (char *) ecalloc(LINELEN, sizeof(char));
		if (buf == NULL) {
			error("can't allocate coordinate input buffer");
		}
	}
	/*
	 * read line
	 */
	if (ugets(fd, buf, LINELEN) == NULL) {
		if (ueof(fd)) {
			return (EOF);
		}

		error("can't read line from coordinate file");
	}
	/*
	 * extract coords (if -g, coords are east, north)
	 */
	if (parm.geoband >= 0) {
		if (sscanf(buf, "%lf %lf", &samp, &line) != 2) {
			error("bad coordinate pair: %s", buf);
		}
	} else {
		if (sscanf(buf, "%lf %lf", &line, &samp) != 2) {
			error("bad coordinate pair: %s", buf);
		}
	}

	/*
	 * if -g option, convert geo coords to line/samp
	 */
	if (parm.geoband >= 0) {

		pt_x = samp; pt_y = line;

		if (pt_x < parm.xmin || pt_x > parm.xmax ||
				pt_y < parm.ymin || pt_y > parm.ymax)  {
			return (ERROR);
		}

		xoffset = parm.dsamp > 0.0 ? pt_x - parm.xmin : parm.xmax - pt_x;
		yoffset = parm.dline > 0.0 ? pt_y - parm.ymin : parm.ymax - pt_y;

		line = (int) (yoffset / ABS(parm.dline));
		samp = (int) (xoffset / ABS(parm.dsamp));

	}
	/*
	 * make sure coords are on the image and sorted by line
	 */
	if (line >= nlines || line < 0 || samp >= nsamps || samp < 0) {
		return (ERROR);
	}	

	if (line < old_line) {
		if (parm.geoband >= 0)
			fprintf(stderr,"\n\tunsorted easting,northing: %g,%g\n",
					pt_x,pt_y);
		error("unsorted line,sample: %g,%g", line, samp);
	}
	old_line = line;

	*linep = line;
	*sampp = samp;

	return (OK);
}
