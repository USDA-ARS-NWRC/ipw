/*
** name
** 	get_sample -- return a sample vector of data
** 
** synopsis
**	int get_sample (fdi, inbuf, mbuf, vector, nbands);
**	int fdi;
**	fpixel_t *inbuf;
**	pixel_t *mbuf;
**	double *vector;
**	int nbands;
** 
** description
**	get_sample returns a {vector} of {nbands} bands of data from the
**	current sample in the input buffer {inbuf}.  If any of the bands
**	are classified (CRH present), the representative value for
**	classified pixel is returned.
** 
** return value
**	Number of bands returned with data.  If the corresponding sample in
**	the mask {mbuf} is non-zero, then 0 is returned.
*/

#include "ipw.h"
#include "crh.h"

#define NO_DATA_CLASS		0

int
get_sample(
	int		 fdi,		/* input image file desc	*/
	fpixel_t	*inbuf,		/* IPW line of data		*/
	pixel_t		*mbuf,		/* mask line of data		*/
	double		*vector,	/* output vector of data	*/
	int		 nbands)	/* # bands in vector		*/
{
	CRH_T          **crhpp;		/* -> CRH of input image	*/
	int		 band;		/* band loop counter		*/
	int		 data_count;	/* # bands with data		*/


	data_count = 0;

   /* check mask for this pixel */

	if (mbuf != NULL) {
		if (*mbuf == 0)
			return (0);
	}

   /* check for presence of CRH */

	crhpp = crh (fdi);

	for (band = 0; band < nbands; band++) {

		if (crhpp == NULL) {
			vector[band] = *inbuf;
			data_count++;

		} else if (crhpp[band] == NULL) {
			vector[band] = *inbuf;
			data_count++;

		} else { /* this band is classified */

			assert (*inbuf <= crh_nclass(crhpp[band]) &&
				*inbuf >= 0);
			if (*inbuf != NO_DATA_CLASS) {
				vector[band] = crh_rep (crhpp[band],
							(int) ((*inbuf)-1));
				data_count++;
			}
		}

		inbuf++;
	}

	return (data_count);
}
