/*
** NAME
**      get_sn_rec -- get the next snow-properties record
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	get_sn_rec(void)
**
** DESCRIPTION
**      This routine loads the next snow-properties record into the
**	proper snow variables.  Before loading the next record though,
**	it computes the difference between the current snow properties
**	(predicted) and those in the next record (measured).  It then
**	reads next record from either the corresponding input file or
**	standard input.  If there are no more records are available, the
**	global variable "more_sn_recs" is set to FALSE.
**
** GLOBAL VARIABLES READ
**	sn_file
**	sn_filename
**	snf
**	temps_in_C
**
** GLOBAL VARIABLES MODIFIED
**	curr_time_hrs
**	current_time
**	h2o_sat
**	more_sn_recs
**	rho
**	start_time
**	T_s
**	T_s_0
**	T_s_l
**	time_s
**	z_s
*/

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"

/*      static variables to hold the next sn record     */
static  double  next_z_s;		/* next snowcover depth */
static  double  next_rho;		/* next snowcover density */
static  double  next_T_s_0;		/* next active snow layer temp */
static  double  next_T_s;	 	/* next snowcover temp */
static  double  next_h2o_sat;		/* next % of liquid h2o saturation */

static int	first_rec = TRUE;	/* reading the 1st record? */


void
get_sn_rec(void)

{
	int	in_count;	/* input count */

	/*
	 *  Read the first record if necessary, and store it as the next rec.
	 */
	if (first_rec) {
		if (sn_file) {
			in_count = fscanf(snf, "%lf %lf %lf %lf %lf %lf",
					 &time_s, &next_z_s, &next_rho,
					 &next_T_s_0, &next_T_s, &next_h2o_sat);
			if (in_count != 6)
				error("missing or bad data in %s",sn_filename);
			check_range (time_s, 0.0, 2400.0, "time_s");
			check_range (next_z_s, 0.0, 50.0, "z_s");
			check_range (next_rho, 0.0, 1000.0, "rho");
			if (temps_in_C) {
				check_range (next_T_s_0, -75.0, 0.0,
					    "T_s_0");
				next_T_s_0 = C_TO_K(next_T_s_0);
				check_range (next_T_s, -75.0, 0.0,
					    "T_s");
				next_T_s   = C_TO_K(next_T_s);
			}
			else {
				check_range (next_T_s_0, 200.0, FREEZE,
					    "T_s_0");
				check_range (next_T_s, 200.0, FREEZE,
					    "T_s");
			}
			check_range (next_h2o_sat, 0.0, 1.0, "h2o_sat");
		}
		else {
			fprintf(stderr,
				"Input initial snow layer properties:\n");
			time_s     = getf("start time (decimal hrs)",
					  0.0, 2400.0);
			next_z_s   = getf("snowcover depth (m)",
					  0.0, 50.0);
			next_rho   = getf("snowcover density (kg/m^3)",
					  0.0, 1000.0);
			if (temps_in_C) {
				next_T_s_0 = getf("snow surface layer temp (C)",
					  	  -75.0, 0.0);
				next_T_s_0 = C_TO_K(next_T_s_0);
				next_T_s   = getf("snowcover temperature (C)",
						  -75.0, 0.0);
				next_T_s   = C_TO_K(next_T_s);
			}
			else {
				next_T_s_0 = getf("snow surface layer temp (K)",
					  	  200.0, FREEZE);
				next_T_s   = getf("snowcover temperature (K)",
						  200.0, FREEZE);
			}
			next_h2o_sat = getf("% liquid h2o saturation",
					    0.0, 1.0);
		}
		curr_time_hrs = time_s;
		time_s = HR_TO_SEC(time_s);
		start_time = current_time = time_s;

		more_sn_recs = TRUE;
	}


	/*
	 *  Report the differences in current snow properties and
	 *  those in the next record.
	 */
	if (first_rec)
		first_rec = FALSE;
	else
		sn_err(next_z_s, next_rho, next_T_s_0, next_T_s, next_h2o_sat);
	
	/*
	 *  Load the values from the next sn record.
	 */
	z_s = next_z_s;
	rho = next_rho;
	T_s = next_T_s;
	T_s_0 = next_T_s_0;
	h2o_sat = next_h2o_sat;

	T_s_l = next_T_s;

	/*
	 *  Setup the snowcover based on these new values.
	 */
	init_snow();

	/*
	 *  Now, read the next sn record.
	 */
	if (sn_file) {
		in_count = fscanf(snf, "%lf %lf %lf %lf %lf %lf", &time_s,
				  &next_z_s, &next_rho, &next_T_s_0, &next_T_s,
				  &next_h2o_sat);
		if (in_count == EOF) {
			more_sn_recs = FALSE;
		}
		else if (in_count != 6) {
			error("missing or bad data in %s", sn_filename);
		}
		else {
			check_range (time_s, curr_time_hrs,
				    curr_time_hrs + 2400.0, "time_s");
			check_range (next_z_s, 0.0, 50.0, "z_s");
			check_range (next_rho, 0.0, 1000.0, "rho");
			if (temps_in_C) {
				check_range (next_T_s_0, -75.0, 0.0,
					    "T_s_0");
				next_T_s_0 = C_TO_K(next_T_s_0);
				check_range (next_T_s, -75.0, 0.0,
					    "T_s");
				next_T_s   = C_TO_K(next_T_s);
			}
			else {
				check_range (next_T_s_0, 200.0, FREEZE,
					    "T_s_0");
				check_range (next_T_s, 200.0, FREEZE,
					    "T_s");
			}
			check_range (next_h2o_sat, 0.0, 1.0, "h2o_sat");

			time_s = HR_TO_SEC(time_s);
		}
	}
	else {
       		if (getok("are there more snow-properties data?")) {
	       		fprintf(stderr,
				"Input updated snow-properties data:\n");
	       		time_s = getf("update time (decimal hrs)", 
				   	(float)curr_time_hrs, 
					(float)(curr_time_hrs + 2400.0));
			time_s = HR_TO_SEC(time_s);

			next_z_s   = getf("snowcover depth (m)",
				  	  0.0, 50.0);
			next_rho   = getf("snowcover density (kg/m^3)",
				  	  0.0, 1000.0);
			if (temps_in_C) {
				next_T_s_0 = getf("snow surface layer temp (C)",
					  	  -75.0, 0.0);
				next_T_s_0 = C_TO_K(next_T_s_0);
				next_T_s   = getf("snowcover temperature (C)",
						  -75.0, 0.0);
				next_T_s   = C_TO_K(next_T_s);
			}
			else {
				next_T_s_0 = getf("snow surface layer temp (K)",
					  	  200.0, FREEZE);
				next_T_s   = getf("snowcover temperature (K)",
						  200.0, FREEZE);
			}
			next_h2o_sat = getf("% liquid h2o saturation",
				    	    0.0, 1.0);
       		}
		else
	       		more_sn_recs = FALSE;
	}
}
