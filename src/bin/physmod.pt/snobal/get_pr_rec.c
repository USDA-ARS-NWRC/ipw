/*
** NAME
**      get_pr_rec -- get the next precipitation record
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	get_pr_rec(void)
**
** DESCRIPTION
**      This routine reads the next precipitation record into the
**	proper precip variables.  This record is read from either
**	the corresponding input file or standard input.  If there
**	are no more records are available, the global variable
**	"more_pr_recs" is set to FALSE.
**
** GLOBAL VARIABLES READ
**	curr_time_hrs
**	pr_file
**	pr_filename
**	prf
**	temps_in_C
**
** GLOBAL VARIABLES MODIFIED
**	m_pp
**	more_pr_recs
**	percent_snow
**	rho_snow
**	T_pp
**	time_pp
*/

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"

int	first_rec = TRUE;	/* reading the 1st record? */


void
get_pr_rec(void)

{
	int	in_count;	/* input count */
	double	mass_rain;	/* mass of rain in precipitation */

	if (first_rec) {
		more_pr_recs = TRUE;
	}

	/*
	 *  Read the next pr record.
	 */
	if (pr_file) {
		in_count = fscanf(prf, "%lf %lf %lf %lf %lf", &time_pp,
				  &m_pp, &percent_snow, &rho_snow, &T_pp);
		if (in_count == EOF) {
			more_pr_recs = FALSE;
		}
		else if (in_count != 5) {
			error ("missing or bad data in %s", pr_filename);
		}
		else {
			check_range(time_pp, curr_time_hrs,
				    curr_time_hrs + 2400.0, "time_pp");
			time_pp = HR_TO_SEC(time_pp);

			check_range(m_pp, 0.0, 50.0, "m_pp");
			check_range(percent_snow, 0.0, 1.0, "%_snow");
			check_range(rho_snow, 0.0, 1000.0, "rho_snow");
			if (temps_in_C) {
				check_range(T_pp, -75.0, 125.0, "T_pp");
				T_pp = C_TO_K(T_pp);
			}
			else
				check_range(T_pp, 200.0, 400.0, "T_pp");
		}
	}
	else {
		if (!first_rec) {
			more_pr_recs = getok("are there more precip data?");
		}

		if (more_pr_recs) {
               		fprintf(stderr,
                                "Input updated precipitation data:\n");
               		time_pp = getf("time of precipitation (decimal hrs)", 
				   	(float)curr_time_hrs,
					(float)(curr_time_hrs + 2400.0));
			time_pp = HR_TO_SEC(time_pp);

       			m_pp = getf("total precip. mass (kg/m^2)", 0.0, 50.0);
       			percent_snow = getf("% of mass that's snow (0 to 1.0)",
					    0.0, 1.0);
       			rho_snow = getf("snow density (kg/m^3)", 0.0, 1000.0);
			if (temps_in_C) {
	       			T_pp = getf("precipitation temperature (C)", 
				            -75.0, 125.0);
				T_pp = C_TO_K(T_pp);
			}
			else
	       			T_pp = getf("precipitation temperature (K)", 
				            200.0, 400.0);
       		}
	}

	/*
	 *  Calculate properties for rain and snow portion of precipitation.
	 */
	if (more_pr_recs) {
		mass_rain = m_pp - percent_snow * m_pp;

		/*
		 *  Precip. temp. cannot be below freezing if rain present.
		 */
		if ((mass_rain > 0.0) && (T_pp < FREEZE)) {
			if (T_pp < (FREEZE - 0.5))
				warn("T_pp < 0.0 C with rain; setting T_pp to 0.0 C");
			T_pp = FREEZE;
		}

	}

	if (first_rec)
		first_rec = FALSE;
}
