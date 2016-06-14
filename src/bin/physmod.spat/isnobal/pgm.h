/*
 ** NAME
 **      pgm.h
 **
 ** DESCRIPTION
 **      The include file for 'isnobal'.
 */

#ifndef _ISNOBAL_H_
#define _ISNOBAL_H_

#include "IPWmacros.h"

#include "bih.h"	/* for BIH_T */
#include "lqh.h"	/* for LQH_T */
#include "geoh.h"	/* for GEOH_T */
#include "units.h"	/* for UNITS_T */


#define DEFAULT_COMPRESS "gzip"	/* default command to compress images    */

#define DEFAULT_Z_U	5.0	/* default wind speed measurement height */
#define DEFAULT_Z_T	5.0	/* default air temp and vapor press hght */

#define IBANDS		6	/* # bands in input image	 	 */
#define EMBANDS	       10	/* # bands in energy/mass output image	 */
#define SBANDS	        9	/* # bands in snow output image		 */
#define PBANDS		4	/* # bands in precip image		 */
#define ICBANDS		7	/* # bands in initial conditions image   */
#define ICBANDS_RESTART	8	/* # bands in init cond image (restart)  */
#define TBANDS	       17	/* # bands in temporary results file	 */

#define NO_DATA   -999999	/* output value for masked pnt (no data) */

typedef struct {
	bool_t masked;
	double elevation;
	double z_0;
	double z_s;
	double rho;
	double T_s_0;			/* active snow layer temp (K) */
	double T_s_l;			/* lower layer temp (C) */
	double T_s;
	double h2o_sat;
	int layer_count;
	double R_n_bar;
	double H_bar;
	double L_v_E_bar;
	double G_bar;
	double M_bar;
	double delta_Q_bar;
	double E_s_sum;
	double melt_sum;
	double ro_pred_sum;
} OUTPUT_REC;
//typedef OUTPUT_REC *out_p;
//extern OUTPUT_REC output_rec[100];	/* output data structure */

/* ------------------------------------------------------------------------- */

/*
 *  Routines that are part of isnobal program.
 */

extern	void	assign_buffers (int masked, int n, int output, OUTPUT_REC **output_rec);
extern	void	buffers        (void);
extern	void	check_range    (int index, double value, double min, double max,
		char * descrip, bool_t print_line_samp);
extern	void	check_units    (LQH_T **lq_headers, UNITS_T *units, int nbands,
		int fd);
extern	void	copy_image     (char *tempfile, int nbands, fpixel_t * buf,
		int fdo);
extern	void 	e_m_image      (int step, OUTPUT_REC **output_rec, int nbits);
extern	bool_t	extract_data   (bool_t first_step, int n, bool_t sun_up[], OUTPUT_REC **output_rec);
extern	void	headers        (void);
extern	void	isnobal		   (int out_step, int nthreads, int dynamic_teams, int got_opt_F, int verbose, int nbits);
/*extern	void	isnobal        (int out_step);*/
extern	void	newlqh         (int fdo, int nbands, fpixel_t *mins,
		fpixel_t *maxs, char **units);
extern	int	open_input     (char *prefix, int index, bool_t *sun_up);
extern	int	output_image   (char * filename, int nbands, char ** units,
		char ** annots, fpixel_t * mins,
		fpixel_t * maxs, int nbits);
extern	bool_t	precip_event   (float curr_time, char *pre_img);
extern	void	precip_hdrs    (char *filename);
extern	void	read_data      (int first_step);
extern	void 	snow_image     (int step, OUTPUT_REC **output_rec, int nbits);
extern	void	temp_filename  (char *prefix, char *filename);
extern	void	warn_range    (int index, double value, double min, double max,
		char * descrip, bool_t print_line_samp);
extern	void	write_data     (int output, int last_step);

/* ------------------------------------------------------------------------- */

/*
 *  Global variables internal to isnobal program.
 */

extern	int	 	units_warn;	/* check units in input images?     */
extern	char	       *compress_cmd;	/* shell command to compress images */

/* timesteps and indices */

extern	int		start_step;	/* index of first timestep	     */
extern	int		nstep;		/* #  of data timesteps		     */
extern	int		nDigits;	/* #  of digits in suffixes of images*/
extern	bool_t		restart;	/* restart flag			     */

/* file variables */

extern	int		fdic;		/* initial conditions image	     */
extern	fpixel_t       *icbuf;		/* initial conditions image buffer   */
//extern	fpixel_t       *icbuf_p;	/* -> into init-cond image buffer    */
extern	BIH_T         **ic_bih;		/* -> to BIH of init-cond image      */
extern	GEOH_T        **ic_geoh;	/* -> to GEOH of init-cond image     */

extern	char	       *in_prefix;	/* prefix for input/update images    */
extern	int		fdi1;		/* file desc for input image 1       */
extern	int		fdi2;		/*  "    "    "    "     "   2       */
extern	fpixel_t       *ibuf1;		/* line buffer for input image 1     */
extern	fpixel_t       *ibuf2;		/*  "     "     "    "     "   2     */
//extern	fpixel_t       *ibuf1_p;	/* -> into line buff for input img 1 */
//extern	fpixel_t       *ibuf2_p;	/* ->  "    "    "    "    "    "  2 */

//extern	int		line;		/* current line in input image	     */
//extern	int		samp;		/* current sample in input image     */

extern	int		fdm;		/* mask img file desc		     */
extern	pixel_t        *mbuf;		/* mask line buffer		     */
//extern	pixel_t        *mbuf_p;		/* -> into mask line buffer	     */

extern	FILE	       *pfp;		/* precip file ptr		     */
extern	int		fdp;		/* precip image file desc	     */
extern	fpixel_t       *pbuf;		/* precip image line buffer	     */
//extern	fpixel_t       *pbuf_p;		/* -> into precip image line buffer  */

extern	int		out_step;	/* time steps per intermediate output*/

extern	char	       *em_prefix;	/* prefix: energy/mass output images */
extern	int		fdem;		/* file desc for temp e/m output file*/
extern	fpixel_t       *embuf;		/* e/m output image line buffer	     */
//extern	fpixel_t       *embuf_p;	/* -> into e/m output img line buff  */
extern	fpixel_t	em_mins[];	/* min of each e/m output image band */
extern	fpixel_t	em_maxs[];	/* max of each e/m output image band */
extern	bool_t	        first_em_pix;	/* first pixel in e/m output img?    */


extern	char	       *snow_prefix;	/* prefix: snow-cond. output images  */
extern	int		fds;		/* file desc for temp snow output    */
extern	fpixel_t       *sbuf;		/* snow output image line buffer     */
//extern	fpixel_t       *sbuf_p;		/* -> into snow output img line buff */
extern	int		nsamps;		/* # samps in snow output image	     */
extern	int		nlines;		/* # lines in snow outpu/t image	     */
extern	bool_t		first_snow_pix;	/* first pixel in snow output?	     */
extern	fpixel_t 	s_mins[];	/* min of each snow output img band  */
extern	fpixel_t 	s_maxs[];	/* max of each snow output img band  */

extern	int		fdti;		/* file desc for temp results input  */
extern	int		fdto;		/* file desc for temp results output */
extern	double	       *itbuf;		/* temp results line buffer (input)  */
extern	double	       *otbuf;		/* temp results line buffer (output) */
//extern	double	       *itbuf_p;	/* -> into temp input line buffer    */
//extern	double	       *otbuf_p;	/* -> into temp output line buffer   */
//extern	int		ot_nbytes;	/* # of bytes in temp out line buff  */



/* model variables */

extern	double		elevation;	/* pixel elevation (m)		     */

#pragma omp threadprivate(elevation)

#endif /* _ISNOBAL_H_ */
