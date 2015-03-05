/*
** NAME
**      get_mh_rec -- get the next measurement-heights (& depths) record
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	get_mh_rec(void)
**
** DESCRIPTION
**      This routine loads the next measurement-heights record into
**	the proper mh variables.  It then reads the next record from
**	either the corresponding input file or standard input.  If
**	there are no more records are available, the global variable
**	"more_mh_recs" is set to FALSE.
**
** GLOBAL VARIABLES READ
**	curr_time_hrs
**	mh_file
**	mh_filename
**	mhf
**	start_time
**
** GLOBAL VARIABLES MODIFIED
**	more_mh_recs
**	time_z
**	z_0
**	z_g
**	z_u
**	z_T
*/

#include "ipw.h"
#include "pgm.h"

/*      static variables to hold the next mh record     */
static double   next_z_u;           	/* next z_u */
static double   next_z_T;           	/* next z_T */
static double   next_z_0;           	/* next z_0 */
static double   next_z_g;           	/* next z_g */

static int	first_rec = TRUE;	/* reading the 1st record? */


void
get_mh_rec(void)

{
	int	in_count;	/* input count */

        /*
	 *  Read the first record if necessary, and store it as the next rec.
	 */
	if (first_rec) {
		if (mh_file) {
			in_count = fscanf(mhf, "%lf %lf %lf %lf %lf", &time_z,
					  &next_z_u, &next_z_T, &next_z_0,
					  &next_z_g);
			if (in_count != 5)
				error ("missing or bad data in %s",
					mh_filename);
			check_range (time_z, (float) SEC_TO_HR(start_time),
				     2400.0, "time_z");
			time_z = HR_TO_SEC(time_z);
			check_range (next_z_u, 0.0, 50.0, "z_u");
			check_range (next_z_T, 0.0, 50.0, "z_T");
			check_range (next_z_0, 0.0, 50.0, "z_0");
			check_range (next_z_g, 0.0,  1.0, "z_g");
		}
		else {
                	time_z = start_time;
                        fprintf(stderr,
                                "Input starting measurement height data:\n");
                	next_z_u = getf("wind measurement height (m)",
					0.0, 50.0);
                	next_z_T = getf("temperature measurement height (m)",
					0.0, 50.0);
                	next_z_0 = getf("surface roughness length (m)",
			 		0.0, 5.0);
                	next_z_g = getf("soil-temp measurement depth (m)",
			 		0.0, 1.0);
		}
		first_rec = FALSE;
		more_mh_recs = TRUE;
	}


	/*
	 *  Load the values from the next mh record.
	 */
       	z_u = next_z_u;
       	z_T = next_z_T;
       	z_0 = next_z_0;
       	z_g = next_z_g;

	/*
	 *  Now, read the next mh record.
	 */
	if (mh_file) {
		in_count = fscanf(mhf, "%lf %lf %lf %lf %lf", &time_z,
				  &next_z_u, &next_z_T, &next_z_0, &next_z_g);
		if (in_count == EOF) {
			more_mh_recs = FALSE;
		}
		else if (in_count != 5) {
			error ("missing or bad data in %s", mh_filename);
		}
		else {
			check_range(time_z, curr_time_hrs, 
				    (curr_time_hrs + 2400.0), "time_z");
			time_z = HR_TO_SEC(time_z);

			check_range(next_z_u, 0.0, 50.0, "z_u");
			check_range(next_z_T, 0.0, 50.0, "z_T");
			check_range(next_z_0, 0.0, 50.0, "z_0");
			check_range(next_z_g, 0.0,  1.0, "z_g");
		}
	}
	else {
       		if (getok("are there more measurement-height data?")) {
               		fprintf(stderr,
                                "Input updated measurement-height data:\n");
               		time_z = getf("update time (decimal hrs)", 
				   	(float)curr_time_hrs, 
					(float)(curr_time_hrs + 2400.0));
			time_z = HR_TO_SEC(time_z);

       			next_z_u = getf("wind measurement height (m)", 
			                0.0, 50.0);
       			next_z_T = getf("temperature meas. height (m)", 
			                0.0, 50.0);
       			next_z_0 = getf("surface roughness length (m)", 
			                0.0, 5.0);
       			next_z_g = getf("soil-temp measurement depth (m)", 
			                0.0, 1.0);
       		}
		else
               		more_mh_recs = FALSE;
	}
}
