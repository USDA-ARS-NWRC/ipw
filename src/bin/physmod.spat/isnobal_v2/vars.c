/*
 *  Global variables internal to isnobal program.
 */

#include "ipw.h"
#include "pgm.h"

	int	 	units_warn;	/* check units in input images?     */
	char	       *compress_cmd;	/* shell command to compress images */

    /* timesteps and indices */ 

	int		start_step;	/* index of first timestep	     */
	int		nstep;		/* #  of data timesteps		     */
	int		nDigits;	/* #  of digits in suffixes of images*/
	bool_t		restart;	/* restart flag			     */

    /* file variables */

	int		fdic;		/* initial conditions image	     */
	fpixel_t       *icbuf;		/* initial conditions image buffer   */
//	fpixel_t       *icbuf_p;	/* -> into init-cond image buffer    */
	BIH_T         **ic_bih;		/* -> to BIH of init-cond image      */
	GEOH_T        **ic_geoh;	/* -> to GEOH of init-cond image     */

	char	       *in_prefix;	/* prefix for input/update images    */
	int		fdi1;		/* file desc for input image 1       */
	int		fdi2;		/*  "    "    "    "     "   2       */
	fpixel_t       *ibuf1;		/* line buffer for input image 1     */
	fpixel_t       *ibuf2;		/*  "     "     "    "     "   2     */
//	fpixel_t       *ibuf1_p;	/* -> into line buff for input img 1 */
//	fpixel_t       *ibuf2_p;	/* ->  "    "    "    "    "    "  2 */

	int		line;		/* current line in input image	     */
	int		samp;		/* current sample in input image     */

	int		fdm;		/* mask img file desc		     */
	pixel_t        *mbuf;		/* mask line buffer		     */
//	pixel_t        *mbuf_p;		/* -> into mask line buffer	     */

	FILE	       *pfp;		/* precip file ptr		     */
	int		fdp;		/* precip image file desc	     */
	fpixel_t       *pbuf;		/* precip image line buffer	     */
//	fpixel_t       *pbuf_p;		/* -> into precip image line buffer  */

	int		out_step;	/* time steps per intermediate output*/

	char	       *em_prefix;	/* prefix: energy/mass output images */
	int		fdem;		/* file desc for temp e/m output file*/
	fpixel_t       *embuf;		/* e/m output image line buffer	     */
//	fpixel_t       *embuf_p;	/* -> into e/m output img line buff  */
	fpixel_t	em_mins[EMBANDS]; /* min of each e/m output img band */
	fpixel_t	em_maxs[EMBANDS]; /* max of each e/m output img band */
	bool_t	        first_em_pix;	/* first pixel in e/m output img?    */


	char	       *snow_prefix;	/* prefix: snow-cond. output images  */
	int		fds;		/* file desc for temp snow output    */
	fpixel_t       *sbuf;		/* snow output image line buffer     */
//	fpixel_t       *sbuf_p;		/* -> into snow output img line buff */
	int		nsamps;		/* # samps in snow output image	     */
	int		nlines;		/* # lines in snow output image	     */
	bool_t		first_snow_pix;	/* first pixel in snow output?	     */
	fpixel_t 	s_mins[SBANDS];	/* min of each snow output img band  */
	fpixel_t 	s_maxs[SBANDS];	/* max of each snow output img band  */

	int		fdti;		/* file desc for temp results input  */
	int		fdto;		/* file desc for temp results output */
	double	       *itbuf;		/* temp results line buffer (input)  */
	double	       *otbuf;		/* temp results line buffer (output) */
//	double	       *itbuf_p;	/* -> into temp input line buffer    */
//	double	       *otbuf_p;	/* -> into temp output line buffer   */
//	int		ot_nbytes;	/* # of bytes in temp out line buff  */
	OUTPUT_REC output_rec[1];	/* output data structure */

    /* model variables */

	double		elevation;	/* pixel elevation (m)		     */
