/*
** NAME
** 	transcoord -- read input file and write translated coords to stdout
** 
** SYNOPSIS
**	void
**	transcoord (ifp, iproj, oproj)
**	FILE *ifp;
**	struct proj *iproj, *oproj;
** 
** DESCRIPTION
**	transcoord reads the given input file, translates the coordinates
**	with the USGS General Cartographic Transformation Package
**	according to iproj and oproj, and writes the translated coordinates
**	and remainder of the input line to stdout.
** 
*/

#include "ipw.h"
#include "gctp.h"
#include "mapproj.h"

#include "pgm.h"

void
transcoord (
	FILE	       *ifp,		/* input file pointer		 */
	struct projdef *iproj,		/* input projection parms	 */
	struct projdef *oproj)		/* output projection parms	 */
{
	char		line[BUFSIZ];	/* input line			 */
	double		incoord[2];	/* input coordinates		 */
	double		outcoord[2];	/* output coordinates		 */
	int		prerr;		/* print error flag		 */
	int		prcoord;	/* print coordinate flag	 */
	int		err;		/* error flag			 */
	int		nlines;		/* input line counter		 */
	int		nchars;		/* #chars read by sscanf	 */
	int		indef[3];	/* GCTP proj def array		 */
	int		outdef[3];	/* GCTP proj def array		 */
	FILE	       *errfile;	/* file ptr to error file	 */


	prerr = 0;		/* print errors */
	prcoord = 1;		/* don't print coordinates */

	nlines = 1;

   /* initialize GCTP projection id arrays */

	indef[0] = iproj->id;
	indef[1] = iproj->zone;
	indef[2] = iproj->uid;

	outdef[0] = oproj->id;
	outdef[1] = oproj->zone;
	outdef[2] = oproj->uid;

	errfile = NULL;


   /* read input lines until EOF */

	while (fgets (line, BUFSIZ, ifp) != NULL) {

		/* read coordinates from input line */

		if (sscanf (line, "%lf %lf%n", &incoord[0], &incoord[1], &nchars)
				!= 2) {
			error ("error reading coordinates from input line %d",
				nlines);
		}

		/* call GCTP to transform coordinates */

		gtrnz0_ (incoord, indef, iproj->parms, outcoord, outdef,
			 oproj->parms, errfile, &err);

		/* process error if any */

		if (err != 0) {
			error ("error %d detected in GCTP; error reprojecting coordinates on line %d",
				err, line);
		}

		/* write coordinates to output file */
		/*
		 *	changed printf format from FLT_DIG to DBL_DIG
		 *	to give more precision in the output.
		 *	D. Marks, July 13, 1995
		*/
		if (oproj->uid == METERS || oproj->uid == FEET) {
			printf ("%.0f %.0f", outcoord[0], outcoord[1]);
		} else {
			printf ("%.*g %.*g", DBL_DIG, outcoord[0],
			 	DBL_DIG, outcoord[1]);
		}

		/* copy remainder of line */

		printf ("%s", &line[nchars]);

		nlines++;
	}
}
