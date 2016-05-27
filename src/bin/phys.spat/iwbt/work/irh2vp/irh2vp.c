/*
**
** NAME
** 	irh2vp -- convert relative humidities to vapor pressure or vice-versa
** 
** SYNOPSIS
** 
**	void irh2vp (fdi, fdm, inverse, clip, fdo)
**	int fdi, fdm, fdo;
**	int inverse, clip;
** 
** DESCRIPTION
** 	irh2vp reads the given input and mask images, converts the relative
**	humidities to vapor pressure (or vice-versa if inverse is set), clips
**	RH to <=1.0, or vp to <= sat_vp if clip is set, and writes the output
**	to the given output image.
** 
*/

#include "ipw.h"
#include "envphys.h"
#include "pgm.h"
#include "fpio.h"
#include "bih.h"
#include "pixio.h"

void
irh2vp (
	int             fdi,		/* input image file descriptor	*/
	int             fdm,		/* mask image file descriptor	*/
	bool_t		inverse,	/* flag for inverse operation	*/
	bool_t		clip,		/* flag for clipping RH <=1.0	*/
	int             fdo)		/* output image file descriptor	*/
{
	pixel_t        *ibuf;		/* -> input line buffer		*/
	pixel_t        *mbuf =NULL;	/* -> mask line buffer		*/
	fpixel_t       *obuf;		/* -> output line buffer	*/
	pixel_t        *ibufp;		/* -> in input buffer		*/
	pixel_t        *mbufp;		/* -> in mask buffer		*/
	fpixel_t       *obufp;		/* -> in output buffer		*/
	fpixel_t      **fmap;		/* -> all input F.P. maps	*/
	fpixel_t       *map0;		/* -> band 0 F.P. map		*/
	fpixel_t       *map1;		/* -> band 1 F.P. map		*/
	int	       *fmaplen;	/* -> lengths of all F.P. maps	*/
	int		maplen;		/* length of band 0 F.P. map	*/
	int		fdt;		/* file desc for temp file	*/
	int		nbytes;		/* # bytes in temp file buffer	*/
	int		nlines;		/* # lines in input image	*/
	int		nsamps;		/* # samples in input image	*/
	int		line;		/* line loop counter		*/
	int		samp;		/* samp loop counter		*/
	int		pixel;		/* pixel loop counter		*/
	double		vp;		/* vapor pressure (Pa)		*/
	double		svp;		/* saturation vapor press (Pa)	*/
	double		rh;		/* relative humidity		*/
	double		min =0.0;	/* min RH or VP in output	*/
	double		max =0.0;	/* max RH or VP in output	*/
	double         *satvp;		/* lookup table for sat VP	*/
	char	       *tempfile;	/* name of temp output file	*/
	bool_t		first_pixel;	/* flag for first pixel		*/


	nsamps = hnsamps (fdi);
	nlines = hnlines (fdi);
	nbytes = nsamps * sizeof(fpixel_t);


   /* open temporary output file */

	tempfile = mktemplate("irh2vp");
	if (tempfile == NULL) {
		error ("Can't get name for temporary file");
	}
	fdt = uwopen_temp(tempfile);
	if (fdt == ERROR) {
		error ("Can't open temporary file");
	}

   /* allocate buffer for input image line */

	ibuf = (pixel_t *) ecalloc (nsamps * IBANDS, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("Unable to allocate input line buffer");
	}

   /* allocate buffer for mask image line */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("Unable to allocate mask line buffer");
		}
	}

   /* allocate buffer for output image line */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("Unable to allocate output line buffer");
	}

   /* access input image F.P. maps */

	fmap = fpmap (fdi);
	map0 = fmap[0];
	map1 = fmap[1];
	fmaplen = fpmaplen (fdi);
	maplen = fmaplen[1];

   /* allocate lookup table for saturation vapor pressure from air temp */
	
	satvp = (double *) ecalloc (maplen, sizeof(double));
	if (satvp == NULL) {
		error ("Unable to allocate lookup table for saturation vapor pressure");
	}

   /* fill saturation VP lookup table */

	for (pixel = 0; pixel < maplen; pixel++) {
		satvp[pixel] = sati (map1[pixel] + FREEZE);
	}

	first_pixel = TRUE;

   /* loop on image lines, performing calculations */

	for (line = 0; line < nlines; line++) {

		/* read input image line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("Error reading input image, line %d", line+1);
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("Error reading mask image, line %d", line+1);
			}
		}

		ibufp = ibuf;
		obufp = obuf;
		mbufp = mbuf;

		/* convert RH to VP or vice-versa for each sample in line */

		for (samp = 0; samp < nsamps; samp++) {

			/* if "inverse" set, calculate relative humidity */

			if (fdm == ERROR || *mbufp) {

				if (inverse) {
					vp = map0[*ibufp++];
					svp = satvp[*ibufp++];
					if (clip) {
						if (vp > svp)
							vp = svp * RH_ERR;
						if (vp < VP_MIN)
							vp = VP_MIN;
					} else {
						if ((vp > svp) || (vp < VP_MIN))
							error ("bad input vp");
					}
					*obufp = vp / svp;
	
				/* else, calculate vapor pressure */
		
				} else {
					rh = map0[*ibufp++];
					if (clip) {
						if (rh > RH_MAX)
							rh = RH_MAX;
						if (rh < RH_MIN)
							rh = RH_MIN;
					} else {
						if ((rh > RH_MAX) || (rh < RH_MIN))
							error ("bad input RH");
					}
					*obufp = satvp[*ibufp++] * rh;
				}

				if (first_pixel) {
					first_pixel = FALSE;
					min = *obufp;
					max = *obufp;
				} else {
					min = MIN (min, *obufp);
					max = MAX (max, *obufp);
				}
				obufp++;
				mbufp++;
			} else {
				if (inverse) {
					*obufp++ = RH_MIN;
				} else {
					*obufp++ = VP_MIN;
				}
				ibufp+=2;
				mbufp++;
			}
		}

		/* write line of output image */

		if (uwrite (fdt, (addr_t) obuf, nbytes) != nbytes) {
			error ("write error, line %d", line);
		}
	}

   /* create/write LQH for output image */

	newlqh (fdo, inverse, min, max);

   /* copy temp file to output image */

	uclose (fdt);
	output (tempfile, fdo);
}
