#include <omp.h>
#include <stdio.h>
#include "ipw.h"
#include "pgm.h"
#include "snobal.h"

int
main (
		int             argc,
		char          **argv)
{

	static OPTION_T opt_t = {
			't', "time steps: data [normal, [,medium [,small]]] (minutes)",
			INT_OPTARGS, "timestep",
			REQUIRED, 1, 4
	};

	static OPTION_T opt_T = {
			'T', "run timesteps' thresholds for a layer's mass (kg/m^2)",
			REAL_OPTARGS, "mass threshold",
			OPTIONAL, 1, 3
	};

	static OPTION_T opt_r = {
			'r', "restart model with given index for first timestep",
			INT_OPTARGS, "restart-index",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_n = {
			'n', "# time steps",
			INT_OPTARGS, "#steps",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_I = {
			'I', "initial conditions image",
			STR_OPTARGS, "image",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_i = {
			'i', "filename prefix for input images",
			STR_OPTARGS, "image_prefix",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_p = {
			'p', "precip file",
			STR_OPTARGS, "precip",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_P = {
			'P', "Number of threads, [dynamic]",
			INT_OPTARGS, "threads",
			OPTIONAL, 1, 2
	};

	static OPTION_T opt_m = {
			'm', "mask image",
			STR_OPTARGS, "mask",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_d = {
			'd', "maximum depth for active layer (m)",
			REAL_OPTARGS, "max-active",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_O = {
			'O', "frequency of output images (1 per how many input images)",
			INT_OPTARGS, "out-frequency",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_e = {
			'e', "prefix for energy/mass output images",
			STR_OPTARGS, "em_prefix",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_s = {
			's', "prefix for snow-conditions output images",
			STR_OPTARGS, "snow_prefix",
			REQUIRED, 1, 1
	};

	static OPTION_T opt_h = {
			'h', "relative measurement heights",
	};

	static OPTION_T opt_U = {
			'U', "check units in input images"
	};

	static OPTION_T opt_M = {
			'M', "snowcover's maximum h2o content as volume ratio",
			REAL_OPTARGS, "max-h2o",
			OPTIONAL, 1, 1
	};

	static OPTION_T opt_C = {
			'C', "shell command to compress output images",
			STR_OPTARGS, "compress_cmd",
			OPTIONAL, 0, 1
	};

	static OPTION_T opt_F = {
			'F', "use temp filenames: isnobal.tmp1 isnobal.tmp2 and don't remove them"
	};

	static OPTION_T opt_v = {
			'v', "verbose output of time step"
	};

	static OPTION_T opt_b = {
			'b', "number of bits for output image",
			INT_OPTARGS, "nbits",
			OPTIONAL, 1, 1
	};


	static OPTION_T *optv[] = {
			&opt_t,
			&opt_T,
			&opt_r,
			&opt_n,
			&opt_I,
			&opt_i,
			&opt_p,
			&opt_P,
			&opt_m,
			&opt_d,
			&opt_O,
			&opt_e,
			&opt_s,
			&opt_h,
			&opt_U,
			&opt_M,
			&opt_C,
			&opt_F,
			&opt_v,
			&opt_b,
			0
	};

	int		level;		 /* loop counter */
	int		data_tstep_min;	 /* data timestep as minutes */
	int		norm_tstep_min;	 /* normal run timestep as minutes */
	int		med_tstep_min;	 /* medium run timestep as minutes */
	int		small_tstep_min; /* small run timestep as minutes */
	double		threshold;	 /* timestep's threshold for 
					    layer's mass */
	int nthreads;				/* number of threads to use */
	int dynamic_teams;			/* number for dynamic teams */
	int nbits;					/* number of bits */

	// force flush of stdout
	setbuf(stdout, NULL);

	/* Some initialization first */

	for (level = DATA_TSTEP; level <= SMALL_TSTEP; level++) {
		tstep_info[level].level = level;
		tstep_info[level].output = 0;
	}

	//	max_z_s_0    = DEFAULT_MAX_Z_S_0;
	//	max_z_s_0 = 0.15;
	/*	z_u          = 5.0;	*/
	z_u	     = DEFAULT_Z_U;
	/*	z_T          = 5.0;	*/
	z_T	     = DEFAULT_Z_T;
	//	relative_hts = TRUE;
	z_g          = DEFAULT_Z_G;

	/* begin */

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/* get/check runstring args */

	/*      get input data's time step  */

	/*      check option for maximum active-surface depth */
	if (got_opt(opt_d)) {
		max_z_s_0 = real_arg(opt_d, 0);
		check_range (0, max_z_s_0, .001, 0.5,
				"max active layer depth", FALSE);
	}
	else {
		max_z_s_0 = DEFAULT_MAX_Z_S_0;
	}

	/* Check relative heights */
	if (got_opt(opt_h)) {
		relative_hts = FALSE;
	}
	else {
		relative_hts = TRUE;
	}

	/* check number of threads to use */
	nthreads = 1;
	dynamic_teams = 100;
	if (got_opt(opt_P)) {
		nthreads = int_arg(opt_P, 0);
		if (nthreads > omp_get_max_threads()){
			nthreads = omp_get_max_threads();
			printf("WARNING - maximum number of threads is %i, using %i\n", omp_get_max_threads(), nthreads);
		}

		if (n_args(opt_t) > 1) {
			dynamic_teams = int_arg(opt_P, 1);
		}
	}

	/*
	 *  The input data's time step must be between 1 minute and 6 hours.
	 *  If it is greater than 1 hour, it must be a multiple of 1 hour, e.g.
	 *  2 hours, 3 hours, etc.
	 */
	data_tstep_min = int_arg(opt_t, 0);
	check_range (0, data_tstep_min, 1.0, HR_TO_MIN(6.0),
			"input data's timestep", FALSE);
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
		check_range (0, norm_tstep_min, 1.0,
				(data_tstep_min > 60 ? 60 : data_tstep_min),
				"normal run timestep", FALSE);
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
		check_range (0, med_tstep_min, 1.0, (float) norm_tstep_min,
				"medium run timestep", FALSE);
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
		check_range (0, small_tstep_min, 1.0, (float) med_tstep_min,
				"small run timestep", FALSE);
	}
	else if (med_tstep_min < DEFAULT_SMALL_TSTEP)
		small_tstep_min = med_tstep_min;
	else
		small_tstep_min = DEFAULT_SMALL_TSTEP;
	if (med_tstep_min % small_tstep_min != 0)
		error("Small timestep must divide evenly into medium timestep");

	tstep_info[SMALL_TSTEP].time_step = MIN_TO_SEC(small_tstep_min);
	tstep_info[SMALL_TSTEP].intervals = med_tstep_min / small_tstep_min;


	/* Mass thresholds for run timesteps */

	if (got_opt(opt_T)) {
		threshold = real_arg(opt_T, 0);
		check_range(0, threshold, 0.0, 1000.0,
				"normal run timestep's threshold for layer's mass",
				FALSE);
	}
	else
		threshold = DEFAULT_NORMAL_THRESHOLD;
	tstep_info[NORMAL_TSTEP].threshold = threshold;

	if (n_args(opt_T) > 1) {
		threshold = real_arg(opt_T, 1);
		check_range(0, threshold, 0.0, 1000.0,
				"medium run timestep's threshold for layer's mass",
				FALSE);
	}
	else
		threshold = DEFAULT_MEDIUM_THRESHOLD;
	if (threshold > tstep_info[NORMAL_TSTEP].threshold)
		error("Threshold for medium timestep > threshold for normal timestep");
	tstep_info[MEDIUM_TSTEP].threshold = threshold;

	if (n_args(opt_T) > 2) {
		threshold = real_arg(opt_T, 2);
		check_range(0, threshold, 0.0, 1000.0,
				"small run timestep's threshold for layer's mass",
				FALSE);
	}
	else
		threshold = DEFAULT_SMALL_THRESHOLD;
	if (threshold > tstep_info[MEDIUM_TSTEP].threshold)
		error("Threshold for small timestep > threshold for medium timestep");
	tstep_info[SMALL_TSTEP].threshold = threshold;

	/* Restarting? */

	restart = got_opt(opt_r);
	if (restart) {
		start_step = int_arg(opt_r, 0);
		if (start_step <= 0)
			error("index of first step must be > 0 for restart");
	}
	else
		start_step = 0;

	if (got_opt(opt_n)) {
		nstep = int_arg(opt_n, 0);
		if (nstep <= 0)
			error("# of data timesteps (%d) must be > 0", nstep);
	}
	else
		nstep = 1;

	/* access init conditions file */

	fdic = uropen(str_arg(opt_I, 0));
	if (fdic == ERROR) {
		error("can't open \"%s\"", str_arg(opt_I, 0));
	}

	/* access mask image, if specified */

	if (got_opt (opt_m)) {
		fdm = uropen (str_arg(opt_m, 0));
		if (fdm == ERROR) {
			error("can't open \"%s\"", str_arg(opt_m, 0));
		}
	} else {
		fdm = ERROR;
	}

	/* access precip file, if specified */

	if (got_opt (opt_p)) {
		pfp = fopen(str_arg(opt_p, 0), "r");
		if (pfp == NULL) {
			error("can't open \"%s\"", str_arg(opt_p, 0));
		}
	} else {
		pfp = NULL;
	}

	/* check for intermediate outputs request */

	if (got_opt(opt_O)) {
		out_step = int_arg(opt_O, 0);
		if (out_step <= 0) {
			error("output frequency (%d) must be > 0", out_step);
		}
	}
	else {
		out_step = nstep;
	}

	/* input & output image prefixes */

	in_prefix = str_arg(opt_i, 0);
	em_prefix = str_arg(opt_e, 0);
	snow_prefix = str_arg(opt_s, 0);

	if (STREQ(em_prefix, snow_prefix))
		error("Prefix for energy/mass output images is the same as prefix for snow conditions output images");

	/* do we check for units ? */

	units_warn = got_opt(opt_U);

	/* check option for maximum liquid h2o */

	if (got_opt(opt_M)) {
		max_h2o_vol = real_arg(opt_M, 0);
		check_range(0, max_h2o_vol, .001, 1.0,
				"max liquid h2o content as vol ratio", FALSE);
	}
	else {
		max_h2o_vol = DEFAULT_MAX_H2O_VOL;
	}

	/* do we compress output images ? */
	if (got_opt(opt_C)) {
		if (n_args(opt_C) == 1) {
			compress_cmd = str_arg(opt_C, 0);
		}
		else {
			compress_cmd = DEFAULT_COMPRESS;
		}
	}
	else {
		compress_cmd = NULL;
	}

	/* output images output bits */
	if (got_opt(opt_b)) {
		nbits = int_arg(opt_b, 0);
		if (nbits <= 0)
			error("number of bits (%d) must be > 0", nbits);
	}
	else
		nbits = 8;

	/* process headers */

	headers();

	/* do all the work */

	/*	isnobal(out_step);	*/
	isnobal(out_step, nthreads, dynamic_teams, got_opt(opt_F), got_opt(opt_v), nbits);

	/* all done */

	ipwexit (EXIT_SUCCESS);
}
