/*
** NAME
**	xtract_data -- extract requested HCN data from given line
** 
** SYNOPSIS
**	void
**	xtract_data (line, station_ID, type, year, month, start_day, end_day,
**		     hydro_year, rday, runoff, drain_area, got_loc, easting,
**		     northing)
**	char *line;
**	char *station_ID;
**	char *type;
**	int year;
**	int month;
**	int start_day, end_day
**	bool_t hydro_year;
**	int rday;
**	bool_t runoff;
**	float drain_area;
**	bool_t got_loc;
**	double easting, northing;
** 
** DESCRIPTION
**	xtract_data extracts the data for the requested day from the given
**	data line and calls output_dat to write it to the output files.
*/

#include "ipw.h"
#include "pgm.h"

void
xtract_data (
	char	       *line,		/* line from file		 */
	char	       *station_ID,	/* station ID			 */
	int		type_code,	/* data type code		 */
	int		year,		/* data year			 */
	int		month,		/* data month			 */
	int		start_day,	/* start day of data		 */
	int		end_day,	/* end day of data		 */
	bool_t		hydro_year,	/* flag for water years		 */
	int		rday,		/* requested day or XTRACT_ALL	 */
	bool_t		runoff,		/* flag to extract runoff data	 */
	float		drain_area,	/* drainage area in square miles */
	bool_t		got_loc,	/* flag if location specified	 */
	double		easting,	/* station easting if got_loc	 */
	double		northing)	/* station northing if got_loc	 */
{
	char		sus_val;	/* suspicious value flag	 */
	char		*linep;		/* pointer in line for next read */
	int		data_code;	/* data code (1-9)		 */
	int		data;		/* data values from line	 */
	int		first_day;	/* first day to extract from line*/
	int		yd;		/* day of Julian or water year	 */
	int		yr;		/* calendar or water year	 */
	int		nout;		/* # data points to output 	 */
	int		i;		/* loop counter			 */


	linep = line;

   /* read and output requested day */

	if (rday != XTRACT_ALL) {
		if (rday > numdays(year, month)) {
			return;
		}
		linep += (rday - start_day) * 7;
		first_day = rday;
		nout = 1;
	} else {
		first_day = start_day;
		nout = MIN (8, numdays(year, month) - start_day + 1);
	}

   /* get water year or Julian day for the first day to extract */

	if (hydro_year) {
		yd = waterday (year, month, first_day, &yr);
	} else {
		yd = yearday (year, month, first_day) - yearday (year, 1, 1);
		yr = year;
	}

   /* process requested days of data in this line */

	for (i = 0; i < nout; i++) {
		sscanf (linep, "%c%1d%5d", &sus_val, &data_code, &data);

		output_dat (type_code, yr, yd, station_ID, sus_val, data_code,
			    data, runoff, drain_area, got_loc, easting,
			    northing);

		yd++;
		linep += 7;
	}
}
