/*
 ** NAME
 **      get_in_rec -- reads and checks input record
 **
 ** SYNOPSIS
 **      #include "pgm.h"
 **
 **      int
 **	get_in_rec(
 **	    INPUT_REC *in_rec)	|* -> input record to returned *|
 **
 ** DESCRIPTION
 **      This routine reads in an input record from either the input file
 **      or standard input.
 **
 ** RETURN VALUE
 **	!= EOF		Return value = # of values read.  The values
 **			have been stored in the parameter 'in_rec'.
 **
 **	EOF		End of file reached.
 **
 ** GLOBAL VARIABLES READ
 **	in
 **	in_file
 **	in_filename
 **	ro_data
 **	temps_in_C
 **
 ** GLOBAL VARIABLES MODIFIED
 */

#include "ipw.h"
#include "pgm.h"
#include "envphys.h"

int
get_in_rec(
		INPUT_REC *in_rec)	/* -> input record to returned */

{
	int     in_val;         /* input counter */

	if (in_file) {

		in_val = fscanf(in, "%lf %lf %lf %lf %lf %lf",
				&(in_rec->S_n), &(in_rec->I_lw), &(in_rec->T_a),
				&(in_rec->e_a), &(in_rec->u), &(in_rec->T_g));

		/*      get runoff data if there        */

		if (ro_data)
			in_val += fscanf(in, "%lf", &(in_rec->ro));

		/*      check number of inputs  */

		if (in_val != EOF) {
			if ((ro_data && (in_val != 7))  ||
					(!ro_data && (in_val != 6)))
				error("missing or bad data in %s", in_filename);
			/*	21/09/2007 changed S_n maxval from 700 to 800 (DGM)	*/
			check_range (in_rec->S_n, 0.0, 800.0, "S_n");
			check_range (in_rec->I_lw, 0.0, 600.0, "I_lw");
			if (temps_in_C) {
				check_range (in_rec->T_a, -75.0, 45.0, "T_a");
				in_rec->T_a = C_TO_K(in_rec->T_a);
			}
			else
				check_range (in_rec->T_a, 200.0, 320.0, "T_a");
			check_range (in_rec->e_a, 0.0, 2000.0, "e_a");
			check_range (in_rec->u, 0.0, 500.0, "u");
			if (temps_in_C) {
				check_range (in_rec->T_g, -95.0, 25.0, "T_g");
				in_rec->T_g = C_TO_K(in_rec->T_g);
			}
			else
				check_range (in_rec->T_g, 180.0, 300.0, "T_g");
		}
	}

	else if ((curr_time_hrs == start_time) || (getok("more data?"))) {
		fprintf(stderr, "input energy parameters:\n");

		/*	21/09/2007 changed S_n maxval from 700 to 800 (DGM)	*/
		in_rec->S_n = getf("net solar (W/m^2)", 0.0, 800.0);
		in_rec->I_lw = getf("incoming thermal (W/m^2)", 0.0, 600.0);
		if (temps_in_C) {
			in_rec->T_a = getf("air temperature (C)", -75.0, 45.0);
			in_rec->T_a = C_TO_K(in_rec->T_a);
		}
		else
			in_rec->T_a = getf("air temperature (K)", 200.0, 320.0);
		in_rec->e_a = getf("vapor pressure (Pa)", 0.0, 2000.0);
		in_rec->u = getf("wind speed (m/sec)", 0.0, 500.0);
		if (temps_in_C) {
			in_rec->T_g = getf("soil temperature at -0.5 m (C)",
					-95.0, 25.0);
			in_rec->T_g = C_TO_K(in_rec->T_g);
		}
		else
			in_rec->T_g = getf("soil temperature at -0.5 m (K)",
					180.0, 300.0);


		in_val = 6;

		/*      get runoff data if there        */
		if (ro_data) {
			in_rec->ro = getf("specific discharge (m/sec)", 0.0,
					1.0);
			in_val++;
		}
	}
	else
		in_val = EOF;

	return in_val;
}
