/******************************************************************************
**
** NAME
** 	itrbxfr -- run trubulent transfer over input image
** 
** SYNOPSIS
**	void itrbxfr (fdi, fdm, fdo, delta_t, z)
**	int fdi;
**	int fdm;
**	int fdo;
**	double delta_t;
**	double z;
** 
** DESCRIPTION
** 	itrbxfr runs the turbulent transfer model on each point in the given
**	input image and writes the results to the output image.
** 
*/

#include	<math.h>
#include	"ipw.h"
#include	"pgm.h"
#include	"bih.h"
#include	"pixio.h"
#include	"fpio.h"
#include 	"envphys.h"

#define		TB	288.0
#define		STDLR	-0.0065

void
itrbxfr (
	int	fdi,		/* input image file descriptor		*/
	int	fdm,		/* mask image file descriptor		*/
	int	fdo,		/* output image file descriptor		*/
	double  delta_t,	/* delta time of model run		*/
	double	z)		/* upper height (m)			*/
{
	double	z0;		/* lower height (m)			*/
	double	ta;		/* upper temperature (C)		*/
	double	ts;		/* lower temperature (C)		*/
	double	ea;		/* upper vapro press. (Pa)		*/
	double	es;		/* lower vapro press. (Pa)		*/
	double	u;		/* upper wind speed (m/s)		*/
	double	pa;		/* air pressure (Pa)			*/
	double	h;		/* sens heat flux (+ to surf) (W/m^2)	*/
	double	le;		/* latent heat flux (+ to surf) (W/m^2)	*/
	double	ev_air;		/* mass flux (+ to surf) (kg/m^2/s)	*/
	double  mm;		/* water gain/loss (mm/m^2)		*/
	double  RH;		/* relative humidity			*/
	double  elev;		/* elevation (m)			*/
	double  vector[IBANDS]; /* input image vector			*/
	double	h_min =0.0;	/* min h in output image		*/
	double	h_max =0.0;	/* max h in output image		*/
	double	le_min =0.0;	/* min le in output image		*/
	double	le_max =0.0;	/* max le in output image		*/
	double	mm_min =0.0;	/* min mm in output image		*/
	double	mm_max =0.0;	/* max mm in output image		*/

	fpixel_t	*inbuf;		/* pointer to input buffer	*/
	fpixel_t	*outbuf;	/* pointer to output buffer	*/
	fpixel_t	*outb;		/* pointer in output buffer	*/
	pixel_t		*mbuf;		/* pointer to mask buffer	*/

	int		index;		/* index to input buffer	*/
	int		nsamps;		/* # samples per line		*/
	int		nlines;		/* # lines in image		*/
	int		samp;		/* sample loop counter		*/
	int		line;		/* line loop counter		*/
	int		ier;		/* return code from hle1	*/
	int		first_pixel;	/* flag for first pixel proc`ed */
	int		fdt;		/* temp output file desc	*/
	int		nbytes;		/* #bytes to write		*/

	char	       *tempfile;	/* temporary output file	*/

#ifdef DEBUG
	FILE *dbg1;
	FILE *dbg2;
#endif


	nsamps = hnsamps (fdo);
	nlines = hnlines (fdo);
	nbytes = nsamps * sizeof(fpixel_t) * OBANDS;


   /* open temporary output file */

	tempfile = mktemplate("itrbxfr");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* Allocate input buffer */

	inbuf = (fpixel_t *) ecalloc (nsamps * IBANDS, sizeof(fpixel_t));
	if (inbuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* Allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate input buffer");
		}
	} else {
		mbuf = NULL;
	}

   /* Allocate output buffer */

	outbuf = (fpixel_t *) ecalloc (nsamps * OBANDS, sizeof(fpixel_t));
	if (outbuf == NULL) {
		error ("can't allocate output buffer");
	}

	first_pixel = TRUE;

#ifdef DEBUG
	dbg1 = fopen ("infile.dbg", "w");
	dbg2 = fopen ("outfile.dbg", "w");
#endif
	
   /* read input data and do calculations */

	for (line=0; line < nlines; line++) {

		/* read line of image */

		if (fpvread (fdi, inbuf, nsamps) != nsamps) {
			error ("read error, line %d", line);
		}

		/* read line of mask image */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("read error, line %d", line);
			}
		}

		/* Do calculations on each sample of line */

		index = 0;
		outb = outbuf;

		for (samp=0; samp < nsamps; samp++) {

			if (get_sample (fdi, inbuf, mbuf, samp, &index, vector,
					IBANDS)
				== IBANDS) {

				RH = vector[0];
				ta = vector[1];
				elev = vector[2];
				u = vector[3];
				z0 = vector[4];

   				/* set pa from site elev */

				pa = HYSTAT (SEA_LEVEL, TB, STD_LAPSE,
				     (elev / 1000.0), GRAVITY, MOL_AIR);

		   		/* convert temps to K */

				ta += FREEZE;
				ts = ta;

				/* set es = sat vp at ts */
				/* derive upper vapor pressure */

				es = sati (ts);
				ea = es * RH;

				if ((ier = hle1 (pa, ta, ts, z, ea, es, z, u,
					z, z0, &h, &le, &ev_air)) != 0) {
					error ("bad return code %d from hle1",
						ier);
				}

				mm = ev_air * delta_t;
		
					
#ifdef DEBUG
 				fprintf (dbg1, "%f %f %f %f %f %f %f 0.0\n",
 					z, z0, ta, ts, ea, es, u);
 				fprintf (dbg2, "%f %f %f\n", h, le, mm);
#endif /* DEBUG */
				*outb++ = h;
				*outb++ = le;
				*outb++ = mm;

				/* update min/max */

				if (!first_pixel) {
					h_min = MIN (h_min, h);
					h_max = MAX (h_max, h);
					le_min = MIN (le_min, le);
					le_max = MAX (le_max, le);
					mm_min = MIN (mm_min, mm);
					mm_max = MAX (mm_max, mm);
				} else {
					first_pixel = FALSE;
					h_min = h;
					h_max = h;
					le_min = le;
					le_max = le;
					mm_min = mm;
					mm_max = mm;
				}

			} else {
				*outb++ = NO_DATA;
				*outb++ = NO_DATA;
				*outb++ = NO_DATA;
			}
		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) outbuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

	/* create/write LQH for output image */

	newlqh (fdo, h_min, h_max, le_min, le_max, mm_min, mm_max);

	/* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);

}
