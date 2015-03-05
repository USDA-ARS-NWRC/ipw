/*
** NAME
** 	resamp_line -- resamples line accumulating sums, mins or maxs
** 
** SYNOPSIS
**	void
**	resamp_line (ibuf, fdm, mbuf, i_nsamps, obuf, sums, npix, o_nsamps,
**		     nbands, zsamp, ztype)
**	fpixel_t *ibuf;
**	int fdm;
**	pixel_t *mbuf;
**	int i_nsamps;
**	fpixel_t *obuf;
**	double *sums;
**	int *npix;
**	int o_nsamps;
**	int nbands;
**	int zsamp;
**	int ztype;
** 
** DESCRIPTION
**	resamp_line resamples the given input line, accumulating sums and
**	pixel counts (for mean calculation) or updating min or max of every
**	zsamp pixels in the input buffer.
**
*/

#include "ipw.h"
#include "pgm.h"

void
resamp_line (
	fpixel_t       *ibuf,		/* input buffer			 */
	int		fdm,		/* mask file desc or ERROR if none*/
	pixel_t        *mbuf,		/* mask buffer			 */
	int		i_nsamps,	/* # samples in input image	 */
	fpixel_t       *obuf,		/* output buffer		 */
	double	       *sums,		/* sums of pixels for mean calc	 */
	int	       *npix,		/* # pixels in mean for out pix	 */
	int		o_nsamps,	/* # samples in output image	 */
	int		nbands,		/* # samples in input image	 */
	int		zsamp,		/* sample zoom factor		 */
	int		ztype)		/* zoom type: MIN,MAX,or MEAN	 */
{
	int		samp;		/* sample loop counter		 */
	int		last_samp;	/* last input samp in average	 */
	int		band;		/* band loop counter		 */
	fpixel_t       *ibufp;		/* -> in input buffer		 */
	fpixel_t       *obufp;		/* -> in output buffer		 */
	pixel_t        *mbufp;		/* -> in mask buffer		 */
	int	       *npixp;		/* -> in npix buffer		 */
	double	       *sumsp;		/* -> in sums buffer		 */


	ibufp = ibuf;
	mbufp = mbuf;
	obufp = obuf;
	sumsp = sums;
	npixp = npix;

	last_samp = zsamp - 1;

	for (samp = 0; samp < i_nsamps; samp++) {

		if (fdm == ERROR || *mbufp++) {

			/* initialize if first data in sample */

			if (*npixp <= 0) {
				if (ztype == MEAN_VAL) {
					for (band = 0; band < nbands; band++) {
						sumsp[band] = *ibufp++;
					}
				} else {
					for (band = 0; band < nbands; band++) {
						obufp[band] = *ibufp++;
					}
				}
				*npixp = 1;

			/* else update sum, min, or max */

			} else {

				for (band = 0; band < nbands; band++) {
					switch (ztype) {

					   case MEAN_VAL:
						sumsp[band] += *ibufp;
						break;

					   case MIN_VAL:
						obufp[band] = MIN (obufp[band],
								   *ibufp);
						break;

					   case MAX_VAL:
						obufp[band] = MAX (obufp[band],
								   *ibufp);
						break;
					}
					ibufp++;
				}
				(*npixp)++;
			}
		} else {
			ibufp += nbands;
		}

		/* if last input sample for averaged output sample */
		/* go on to next output sample */

		if (samp >= last_samp) {
			obufp += nbands;
			sumsp += nbands;
			npixp++;
			last_samp = MIN (last_samp+zsamp, i_nsamps-1);
		}
	}
}
