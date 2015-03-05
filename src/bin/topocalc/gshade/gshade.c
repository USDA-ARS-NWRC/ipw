/*
** NAME
** 	gshade -- compute cos local illumination angle for global scale image
** 
** SYNOPSIS
**	void gshade (fdi, fds, fdm, fdo, zenith)
** 	int fdi, fds, fdm, fdo;
**	bool_t zenith;
** 
** DESCRIPTION
** 	gshade reads in the input and solar images, computes the cosine of
**	the local illuminatio angle of each pixel and writes the output image.
** 
*/

#include <math.h>
#include "ipw.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "pgm.h"

void
gshade (
	int             fdi,		/* input file desc		 */
	int             fds,		/* solar file desc		 */
	int             fdm,		/* mask file desc		 */
	int             fdo,		/* output file desc		 */
	bool_t		zenith)		/* flag for zenith ang in solar	 */
{
	int		nsamps;		/* # samples in input image	 */
	int		nlines;		/* # lines in input image	 */
	int             nbands;		/* # input bands		 */
	int		s_nbands;	/* # bands in solar image	 */
	int		line;		/* line loop counter		 */
	int		samp;		/* sample loop counter		 */
	pixel_t	       *ibuf;		/* -> input buffer		 */
	pixel_t	       *sbuf;		/* -> solar image buffer	 */
	pixel_t	       *mbuf =NULL;	/* -> mask image buffer		 */
	fpixel_t       *obuf;		/* -> output buffer		 */
	pixel_t	       *ibufp;		/* -> in input buffer		 */
	pixel_t	       *sbufp;		/* -> in solar image buffer	 */
	pixel_t	       *mbufp;		/* -> in mask image buffer	 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	int            *maplen;		/* lengths of input F.P. maps	 */
	int            *smaplen;	/* lengths of solar F.P. maps	 */
	fpixel_t      **map;		/* input F.P. conversion map	 */
	fpixel_t      **smap;		/* input F.P. conversion map	 */
	float	       *costbl;		/* cos slope lookup table	 */
	fpixel_t       *sintbl;		/* sin slope lookup table	 */
	fpixel_t       *zcostbl;	/* cos zenith ang lookup table	 */
	float	       *zsintbl;	/* sin zenith ang lookup table	 */
	fpixel_t       *azm_map;	/* -> azimuth F.P. map		 */
	fpixel_t       *expo_map;	/* -> exposure F.P. map		 */
	double		azm;		/* solar azimuth		 */
	double		expo;		/* exposure			 */
	double		cosd;		/* cos (azimuth - exposure)	 */
	double		mu;		/* cos local illumination angle	 */
	double		pi;


	pi = M_PI;

	nsamps = hnsamps(fdi);
	nlines = hnlines(fdi);
	nbands = hnbands(fdi);
	s_nbands = hnbands(fds);

   /* allocate input buffer */

	ibuf = (pixel_t *) ecalloc (nbands * nsamps, sizeof(pixel_t));
	if (ibuf == NULL) {
		error ("can't allocate input buffer");
	}

   /* allocate solar buffer */

	sbuf = (pixel_t *) ecalloc (s_nbands * nsamps, sizeof(pixel_t));
	if (sbuf == NULL) {
		error ("can't allocate solar image buffer");
	}

   /* allocate mask buffer */

	if (fdm != ERROR) {
		mbuf = (pixel_t *) ecalloc (nsamps, sizeof(pixel_t));
		if (mbuf == NULL) {
			error ("can't allocate mask image buffer");
		}
	}

   /* allocate output buffer */

	obuf = (fpixel_t *) ecalloc (nsamps, sizeof(fpixel_t));
	if (obuf == NULL) {
		error ("can't allocate output buffer");
	}

   /* buffers to hold sines and cosines of slopes and solar azimuth angles */

	maplen = fpmaplen (fdi);
	map = fpmap (fdi);
	sintbl = map[0];
	expo_map = map[1];
	costbl = (float *) ecalloc (maplen[0], sizeof(float));

	smaplen = fpmaplen (fds);
	smap = fpmap (fds);
	if (!zenith) {
		zcostbl = smap[0];
	} else {
		zcostbl = (float *) ecalloc (smaplen[0], sizeof(float));
	}
	zsintbl = (float *) ecalloc (smaplen[0], sizeof(float));
	azm_map = smap[1];

   /* initialize trig lookup tables */

	trigtbl (fdi, fds, sintbl, costbl, zsintbl, zcostbl, zenith);

   /* loop reading input lines, calculating cosines and writing to output */

	for (line = 0; line < nlines; line++) {

		/* read input line */

		if (pvread (fdi, ibuf, nsamps) != nsamps) {
			error ("error reading input image, line %d", line);
		}

		/* read solar image line */

		if (pvread (fds, sbuf, nsamps) != nsamps) {
			error ("error reading solar image, line %d", line);
		}

		/* read mask image line */

		if (fdm != ERROR) {
			if (pvread (fdm, mbuf, nsamps) != nsamps) {
				error ("error reading mask image, line %d", line);
			}
		}

		/* perform calculation for each sample */

		ibufp = ibuf;
		sbufp = sbuf;
		obufp = obuf;
		mbufp = mbuf;
		for (samp = 0; samp < nsamps; samp++) {

			if (fdm == ERROR || *mbufp) {
				azm = azm_map[*(sbufp+1)] * pi / 180.0;
				expo = expo_map[*(ibufp+1)];
				cosd = cos (azm - expo);
				mu = zcostbl[*sbufp] * costbl[*ibufp] +
					zsintbl[*sbufp] * sintbl[*ibufp] * cosd;
				if (mu < 0)
					mu = 0;
				else if (mu > 1)
					mu = 1;

				*obufp++ = mu;
			} else {
				*obufp++ = 0.0;
			}
			ibufp+=2;
			sbufp+=2;
			mbufp++;
		}

		/* write output buffer */

		if (fpvwrite (fdo, obuf, nsamps) != nsamps) {
			error ("error writing output buffer, line %d", line);
		}
	}
}
