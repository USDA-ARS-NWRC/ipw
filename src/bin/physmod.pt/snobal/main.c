#include	"ipw.h"
#include	"pgm.h"
#include        "snobal.h"

int
main(
		int	argc,
		char	**argv)
{
	/***    get args        ***/

	get_args(argc, argv);

	/***    input and output files  ***/

	open_files();

	/***    get & set initial conditions and constants ***/

	initialize();

	/***    get first climate data record ***/

	if (get_in_rec(&input_rec1) == EOF)
		error("empty input file");
	rec_count = 1;

	/***    loop while reading remaining input records to calculate E.B. ***/

	stop_no_snow = FALSE;

	while ((get_in_rec(&input_rec2) != EOF) && !stop_no_snow) {

		/* increment record counter */
		rec_count++;

		/*  does the next precip record applies to this data tstep? */
		if (precip_data && more_pr_recs && IN_CURR_DATA_TSTEP(time_pp))
			precip_now = TRUE;
		else
			precip_now = FALSE;

		/* run model for 1 data timestep between 2 input records */
		if (! do_data_tstep())
			error("Error at record %d", rec_count);

		/***    update snow properties and measurement heights if needed  ***/

		if (more_sn_recs) 
			if (IN_CURR_DATA_TSTEP(time_s))
				get_sn_rec();

		if (more_mh_recs)
			if (IN_CURR_DATA_TSTEP(time_z))
				get_mh_rec();

		/*	update precip record if current one applied to this timestep */
		if (precip_data && precip_now)
			get_pr_rec();

		/*	copy ending input record into starting input record */
		input_rec1 = input_rec2;
	}  /* loop on input records */

	if (rec_count == 1)
		error("input file has only 1 record");

	/*      stopped because no snow left?  */
	if (stop_no_snow)
		printf("out of snow at rec %d\n", rec_count);

	printf("%d recs processed over %f days\n", rec_count,
			SEC_TO_DAY(rec_count * tstep_info[DATA_TSTEP].time_step) );

	ipwexit(EXIT_SUCCESS);
}
