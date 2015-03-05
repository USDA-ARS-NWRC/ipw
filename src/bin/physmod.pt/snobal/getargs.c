/*
** NAME
**      get_args - gets and checks arguments
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	get_args(
**	    int	  argc,
**	    char  **argv)
**
** DESCRIPTION
**      Reads and checks input arguments for point, 2-layer
**      energy balance snowmelt model.
**
** GLOBAL VARIABLES READ
**
** GLOBAL VARIABLES MODIFIED
**	elevation
**	in_file
**	in_filename
**	max_h2o_vol
**	max_z_s_0
**	mh_file
**	mh_filename
**	out_file
**	out_filename
**	pr_file
**	pr_filename
**	precip_data
**	run_no_snow
**	sn_file	
**	sn_filename
**	temps_in_C	
**	tstep_info	
**
** HISTORY
**      Sept., 1984:	written by D. Marks, CSL, UCSB;
**      July, 1985:	handeling of var defs and externs improved
**              	by D. Marks, CSL, UCSB;
**      April, 1986:	2-layer update by D. Marks, CSL, UCSB;
**	January, 1990:	made less interactive - new input files added
**			by Kelly Longley, OSU;
**	February, 1990:	added argument error range checking by
**			K. Longley, OSU, EPA ERL-C;
**	May 1995	Converted to IPW by J. Domingo, OSU, US EPA
**	Feb 1996	Renamed some options.  Added -c, -K, -Z options.
**			J. Domingo, OSU
**	Mar 1996	Added critical timestep to -t option.  Changed
**			timestep units from hours to minutes.
**			J. Domingo, OSU
*/

#include "ipw.h"
#include "pgm.h"
#include "snobal.h"

void
get_args(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_z = {
		'z', "site elevation (m)",
		REAL_OPTARGS, "elev",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_t = {
		't', "time steps: data [normal, [,medium [,small]]] (minutes)",
		INT_OPTARGS, "timestep",
		REQUIRED, 1, 4
	};

	static OPTION_T opt_m = {
		'm', "snowcover's maximum h2o content as volume ratio",
		REAL_OPTARGS, "max-h2o",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
		'd', "maximum depth for active layer (m)",
		REAL_OPTARGS, "max-active",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_s = {
		's', "snow properties input data file",
		STR_OPTARGS, "snfile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "measurement heights input data file",
		STR_OPTARGS, "mhfile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_p = {
		'p', "precipitation input data file",
		STR_OPTARGS, "prfile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "optional input data file",
		STR_OPTARGS, "infile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "optional output data file",
		STR_OPTARGS, "outfile",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_O = {
		'O', "how often output records written (data, normal, all)",
		STR_OPTARGS, "mode",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "continue run even when no snowcover"
	};

	static OPTION_T opt_K = {
		'K', "accept temperatures in degrees K"
	};

	static OPTION_T opt_T = {
		'T', "run timesteps' thresholds for a layer's mass (kg/m^2)",
		REAL_OPTARGS, "mass threshold",
		OPTIONAL, 1, 3
	};

/*
 *  Internal options for model development
 *
 *	static OPTION_T opt_A = {
 *		'A', "alpha constant for adjusting solar net",
 *		REAL_OPTARGS, "alpha",
 *		REQUIRED, 1, 1
 *	};
 *
 *	static OPTION_T opt_B = {
 *		'B', "beta constant for adjusting solar net",
 *		REAL_OPTARGS, "beta",
 *		REQUIRED, 1, 1
 *	};
 */

	static OPTION_T *optv[] = {
		&opt_z,
		&opt_t,
		&opt_m,
		&opt_d,
		&opt_s,
		&opt_h,
		&opt_p,
		&opt_i,
		&opt_o,
		&opt_O,
		&opt_c,
		&opt_K,
		&opt_T,
/*		&opt_A,		*/
/*		&opt_B,		*/
		0
	};

/*	local variables	  */

	int	level;		  /* loop counter */
	int	data_tstep_min;	  /* data timestep as minutes */
	int	norm_tstep_min;	  /* normal run timestep as minutes */
	int	med_tstep_min;	  /* medium run timestep as minutes */
	int	small_tstep_min;  /* small run timestep as minutes */
	double	threshold;	  /* timestep' threshold for layer's mass */


	/*
	 *  Some initialization first
	 */
	for (level = DATA_TSTEP; level <= SMALL_TSTEP; level++) {
		tstep_info[level].level = level;
		tstep_info[level].output = 0;
	}

/***    get args        ***/

	ipwenter (argc, argv, optv, IPW_DESCRIPTION);

        /*      get elev arg    */

	elevation = real_arg(opt_z, 0);

        /*      get input data's time step  */

	/*
	 *  The input data's time step must be between 1 minute and 6 hours.
	 *  If it is greater than 1 hour, it must be a multiple of 1 hour, e.g.
	 *  2 hours, 3 hours, etc.
	 */
	data_tstep_min = int_arg(opt_t, 0);
	check_range (data_tstep_min, 1.0, HR_TO_MIN(6.0),
		     "input data's timestep");
	if ((data_tstep_min > 60) && (data_tstep_min % 60 != 0))
		error("Data timestep > 60 min must be multiple of 60 min (whole hrs)");
	tstep_info[DATA_TSTEP].time_step = MIN_TO_SEC(data_tstep_min);

	/*
	 *  If the normal run timestep is specified, it must be less than
	 *  the minimum of these two: the data timestep or 60 minutes (1 hr).
	 *  Otherwise, the default normal timestep is 1 hour (for data
	 *  timestep > 1 hr) or the data timestep (for data timestep <= 1 hr).
	 */
	if (n_args(opt_t) > 1) {
		norm_tstep_min = int_arg(opt_t, 1);
		check_range (norm_tstep_min, 1.0,
			     (data_tstep_min > 60 ? 60 : data_tstep_min),
			     "normal run timestep");
	}
	else if (data_tstep_min > 60)
		norm_tstep_min = 60;
	else
		norm_tstep_min = data_tstep_min;
	if (data_tstep_min % norm_tstep_min != 0)
		error("Normal run timestep must divide evenly into data timestep");

	tstep_info[NORMAL_TSTEP].time_step = MIN_TO_SEC(norm_tstep_min);
	tstep_info[NORMAL_TSTEP].intervals = data_tstep_min / norm_tstep_min;

	/*
	 *  The medium run timestep must be divide evenly into the normal
	 *  run timestep.
	 */
	if (n_args(opt_t) > 2) {
		med_tstep_min = int_arg(opt_t, 2);
		check_range (med_tstep_min, 1.0, (float) norm_tstep_min,
			     "medium run timestep");
	}
	else if (norm_tstep_min < DEFAULT_MEDIUM_TSTEP)
		med_tstep_min = norm_tstep_min;
	else
		med_tstep_min = DEFAULT_MEDIUM_TSTEP;
	if (norm_tstep_min % med_tstep_min != 0)
		error("Medium timestep must divide evenly into normal timestep");

	tstep_info[MEDIUM_TSTEP].time_step = MIN_TO_SEC(med_tstep_min);
	tstep_info[MEDIUM_TSTEP].intervals = norm_tstep_min / med_tstep_min;

	/*
	 *  The small run timestep must be divide evenly into the medium
	 *  run timestep.
	 */
	if (n_args(opt_t) > 3) {
		small_tstep_min = int_arg(opt_t, 3);
		check_range (small_tstep_min, 1.0, (float) med_tstep_min,
			     "small run timestep");
	}
	else if (med_tstep_min < DEFAULT_SMALL_TSTEP)
		small_tstep_min = med_tstep_min;
	else
		small_tstep_min = DEFAULT_SMALL_TSTEP;
	if (med_tstep_min % small_tstep_min != 0)
		error("Small timestep must divide evenly into medium timestep");

	tstep_info[SMALL_TSTEP].time_step = MIN_TO_SEC(small_tstep_min);
	tstep_info[SMALL_TSTEP].intervals = med_tstep_min / small_tstep_min;


        /*      check option for maximum liquid h2o */

        if (got_opt(opt_m)) {
                max_h2o_vol = real_arg(opt_m, 0);
		check_range (max_h2o_vol, .001, 1.0,
		             "max liquid h2o content as vol ratio");
        }
        else {
		max_h2o_vol = DEFAULT_MAX_H2O_VOL;
        }

        /*      check option for maximum active-surface depth */

        if (got_opt(opt_d)) {
                max_z_s_0 = real_arg(opt_d, 0);
		check_range (max_z_s_0, .001, 0.5,
		             "max active layer depth");
        }
        else {
                max_z_s_0 = DEFAULT_MAX_Z_S_0;
        }

        /*      check for snow properties input file name       */

	sn_file = got_opt(opt_s);
        if (sn_file) {
                sn_filename = str_arg(opt_s, 0);
        }

        /*      check for measurement heights input file name       */

	mh_file = got_opt(opt_h);
        if (mh_file) {
                mh_filename = str_arg(opt_h, 0);
        }

        /*      check for input file name       */

	in_file = got_opt(opt_i);
        if (in_file) {
                in_filename = str_arg(opt_i, 0);
        }

        /*      check for precipitation input file name       */

	precip_data = pr_file = got_opt(opt_p);
        if (pr_file) {
                pr_filename = str_arg(opt_p, 0);
        }

        /*      check for output file name     */

	out_file = got_opt(opt_o);
        if (out_file) {
                out_filename = str_arg(opt_o, 0);
	}

	/*
	 *  frequency of output records
	 */
	if (got_opt(opt_O)) {
		if (STREQ(str_arg(opt_O, 0), "data"))
			tstep_info[DATA_TSTEP].output = DIVIDED_TSTEP;
		else if (STREQ(str_arg(opt_O, 0), "normal"))
			tstep_info[NORMAL_TSTEP].output = WHOLE_TSTEP |
							  DIVIDED_TSTEP;
		else if (STREQ(str_arg(opt_O, 0), "all")) {
			tstep_info[NORMAL_TSTEP].output = WHOLE_TSTEP;
			tstep_info[MEDIUM_TSTEP].output = WHOLE_TSTEP;
			tstep_info[SMALL_TSTEP].output = WHOLE_TSTEP;
		}
		else
		     error("output frequency must be: data, normal, or all");
	}
	else
		tstep_info[DATA_TSTEP].output = DIVIDED_TSTEP;


	/*	run model even when no snow left?	*/

	run_no_snow = got_opt(opt_c);

	/*	temperatures in degrees C or K?	*/

	temps_in_C = ! got_opt(opt_K);


	/*
	 *  Mass thresholds for run timesteps
	 */
	if (got_opt(opt_T)) {
		threshold = real_arg(opt_T, 0);
		check_range (threshold, 0.0, 1000.0,
			    "normal run timestep's threshold for layer's mass");
	}
	else
		threshold = DEFAULT_NORMAL_THRESHOLD;
	tstep_info[NORMAL_TSTEP].threshold = threshold;

	if (n_args(opt_T) > 1) {
		threshold = real_arg(opt_T, 1);
		check_range (threshold, 0.0, 1000.0,
			    "medium run timestep's threshold for layer's mass");
	}
	else
		threshold = DEFAULT_MEDIUM_TSTEP;
	if (threshold > tstep_info[NORMAL_TSTEP].threshold)
		error("Threshold for medium timestep > threshold for normal timestep");
	tstep_info[MEDIUM_TSTEP].threshold = threshold;

	if (n_args(opt_T) > 2) {
		threshold = real_arg(opt_T, 2);
		check_range (threshold, 0.0, 1000.0,
			    "small run timestep's threshold for layer's mass");
	}
	else
		threshold = DEFAULT_SMALL_TSTEP;
	if (threshold > tstep_info[MEDIUM_TSTEP].threshold)
		error("Threshold for small timestep > threshold for medium timestep");
	tstep_info[SMALL_TSTEP].threshold = threshold;

/*
 *  Alpha & beta determined from test runs.
 *
 *	alpha = real_arg(opt_A, 0);
 *	beta  = real_arg(opt_B, 0);
 */
}
