/*
** NAME
**	output_dat -- output data to the output files
** 
** SYNOPSIS
**	void
**	output_dat (type_code, year, yd, station_ID, sus_val, data_code, data,
**		    runoff, drain_area, got_loc, easting, northing)
**	int type_code;
**	int year;
**	int yd;
**	char *station_ID;
**	char sus_val;
**	int data_code;
**	int data;
**	bool_t runoff;
**	float drain_area;
**	bool_t got_loc;
**	double easting, northing;
** 
** DESCRIPTION
**
**	output_dat converts the units of the given data and writes it to
**	the output file for the given day (see documentation in main.c
**	for output file name, format and conversion types).
*/

#include "ipw.h"
#include "pgm.h"

void
output_dat (
	int		type_code,	/* data type code		 */
	int		year,		/* data year			 */
	int		yd,		/* Julian or water day of year	 */
	char	       *station_ID,	/* station ID			 */
	char		sus_val,	/* suspicious value flag	 */
	int		data_code,	/* data code (1-9)		 */
	int		data,		/* data values from line	 */
	bool_t		runoff,		/* flag to extract runoff data	 */
	float		drain_area,	/* drainage area in square miles */
	bool_t		got_loc,	/* flag if location specified	 */
	double		easting,	/* station easting if got_loc	 */
	double		northing)	/* station northing if got_loc	 */
{
	char		filename[255];	/* output file name		 */
	bool_t		bad_data;	/* flag for bad data		 */
	float		cnv_data =0.0;	/* converted data		 */
	FILE	       *ofp;		/* output file pointer		 */



   /* check validity of data and perform units conversion */

	bad_data = FALSE;

	if (runoff) {
		/* convert cubic feet/sec/square mile to */
		/* millimteters/square meter/day */

		if (data != R_NODATA) {
			cnv_data = ROFF_CFACTOR * data / drain_area;
		} else {
			bad_data = TRUE;
		}

	} else {

		if (sus_val == BLANK) {

			switch (type_code) {
				case PRCP:
					cnv_data = data / 100.0 * MM_PER_INCH;
					break;

				case TMIN:
				case TMAX:
					cnv_data = 5.0 / 9.0 * (data / 100.0
						     - 32.0);
					break;
			}
		} else {
			bad_data = TRUE;
		}
	}


   /* construct output filename */

	if (!bad_data) {
		sprintf (filename, "%s.%d.%03d", types_lc[type_code], year, yd);
	} else {
		sprintf (filename, "%s.%d.errs", types_lc[type_code], year);
	}

   /* open file in append mode */

	ofp = fopen (filename, "a");
	if (ofp == NULL) {
		error ("error opening output file %s", filename);
	}

   /* write data to output file or errs file */

	if (!bad_data) {

		if (got_loc) {
			fprintf (ofp, "%.*g %.*g %.2f\n", FLT_DIG, easting,
				 FLT_DIG, northing, cnv_data);
		} else {
			fprintf (ofp, "%s %.2f\n", station_ID, cnv_data);
		}

   /* write suspicious or missing data to errs file */

	} else {
		fprintf (ofp, "%s %d %d %c %d %d\n",
			 station_ID, year, yd, sus_val, data_code, data);
	}

   /* close output file */

	fclose (ofp);
}
