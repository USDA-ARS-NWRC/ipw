/*
** name
** 	get_sample -- return a sample vector of data for the trbxfr model.
** 
** synopsis
**	get_sample (inbuf, mbuf, samp, buf_index, vector, nbands);
**	fpixel_t *inbuf;
**	pixel_t *mbuf;
**	int samp;
**	int *buf_index;
**	double *vector;
**	int nbands;
** 
** description
**	get_sample returns a vector of data from inbuf[buf_index] for nbands
**	bands.	If any of the bands are classified (CRH present), the
**	representative value for classified pixel is returned.  The buffer
**	index buf_index is updated and returned.
** 
** return value
**	Number of bands returned with data
** 
** history
**	Aug 1995	Fixed bug so that index is ALWAYS incremented
**			for each pass through loop.  J. Domingo, OSU
**
*/

#include "ipw.h"
#include "crh.h"

#define NO_DATA		0

int
get_sample (
	int		 fdi,		/* input image file desc	*/
	fpixel_t	*inbuf,		/* IPW line of data		*/
	pixel_t		*mbuf,		/* mask line of data		*/
	int		 samp,		/* sample index for mask	*/
	int		*buf_index,	/* index in input buffer	*/
	double		*vector,	/* output vector of data	*/
	int		 nbands)	/* # bands in vector		*/
{
	CRH_T          **crhpp;		/* -> CRH of input image	*/
	int		 index;		/* index of sample		*/
	int		 band;		/* band loop counter		*/
	int		 data_count;	/* # bands with data		*/


	index = *buf_index;
	*buf_index += nbands;
	data_count = 0;

   /* check mask for this pixel */

	if (mbuf != NULL) {
		if (mbuf[samp] == 0)
			return (0);
	}

   /* check for presence of CRH */

	crhpp = crh (fdi);

	for (band = 0; band < nbands; band++) {

		if (crhpp == NULL) {
			vector[band] = inbuf[index];
			data_count++;

		} else if (crhpp[band] == NULL) {
			vector[band] = inbuf[index];
			data_count++;

		} else { /* this band is classified */

			assert (inbuf[index] <= crh_nclass(crhpp[band]) &&
				inbuf[index] >= 0);
			if (inbuf[index] != NO_DATA) {
				vector[band] = crh_rep (crhpp[band],
							(int) inbuf[index]-1);
				data_count++;
			}
		}

		index++;
		/*
		 *  Pulled this increment of the index out of each section
		 *  of the if-else if-else statement above.  In the "else"
		 *  section, it was only incremented inside the "if" body
		 *  (where data_count was incremented).  Which meant that if
	  	 *  there was NO_DATA for a classified band, then the index
		 *  was *not* incremented properly.  So for the next band,
		 *  the index was still pointing to the previous band.
		 *  J. Domingo, OSU
		 */
	}

	return (data_count);
}
