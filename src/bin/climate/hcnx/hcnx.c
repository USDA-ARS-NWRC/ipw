/*
** NAME
**	hcnx -- extract requested HCN data from given file and append to data files
** 
** SYNOPSIS
**	hcnx (ifp, ryear, rmonth, rday, hydro_year, xtract, runoff, drain_area,
**	      got_loc, easting, northing)
**	FILE *ifp;
**	int ryear, rmonth, rday;
**	bool_t hydro_year;
**	bool_t *xtract;
**	bool_t runoff;
**	float drain_area;
**	bool_t got_loc;
**	double easting, northing;
** 
** DESCRIPTION
**	hcnx extracts the requested types of data for the requested years
**	from the given input file and appends the data to the output data
**	file(s).
*/

#include "ipw.h"
#include "pgm.h"

void
hcnx (
	FILE	       *ifp,		/* input file pointer		 */
	int		ryear,		/* year to extract or XTRACT_ALL */
	int		rmonth,		/* month to extract or XTRACT_ALL*/
	int		rday,		/* day to extract or XTRACT_ALL  */
	bool_t		hydro_year,	/* flag for water years		 */
	bool_t	       *xtract,		/* flag to extract each data type*/
	bool_t		runoff,		/* flag to extract runoff data	 */
	float		drain_area,	/* drainage area in square miles */
	bool_t		got_loc,	/* flag for location specified	 */
	double		easting,	/* station easting		 */
	double		northing)	/* station northing		 */
{
	int		nlines;		/* # lines read from file	 */
	int		year;		/* data year			 */
	int		month;		/* data month			 */
	int		rec_id;		/* record ID (1-4)		 */
	int		type_code =0;	/* integer code for type	 */
	int		start_day;	/* start day of record		 */
	int		end_day;	/* end day of record		 */
	int		i;		/* loop counter			 */
	char		line[MAXLINE];	/* line from file		 */
	char		station_ID[7];	/* station ID			 */
	char		type[5];	/* data type			 */
	bool_t		do_xtract;	/* flag to extract this record	 */


	nlines = 0;

   /* loop reading lines of data until EOF */

	while (fgets (line, MAXLINE, ifp) != NULL) {

		nlines++;

		/* get station ID, data type, year, month & rec_id from line */

		if (runoff) {

			if (sscanf (line, "%*2c%8s%*6c%4d%2d%2d",
				station_ID, &year, &month, &rec_id) != 4) {
				error ("error reading input file, line %d",
					nlines);
			}
			do_xtract = TRUE;
			type_code = ROFF;

		} else {

			if (sscanf (line, "%6s %4s %4d%2d%2d",
			    	station_ID, type, &year, &month,
					&rec_id) != 5) {
				error ("error reading input file, line %d",
					nlines);
			}

			do_xtract = FALSE;

			/* is this a requested data type? */

			for (i = 0; i < NTYPES; i++) {

				if (streq (types[i], type)) {
					type_code = i;
					if (xtract[type_code]) {
						do_xtract = TRUE;
					}
					break;
				}
			}
		}

		/* is this a requested year or month? */

		if (ryear != XTRACT_ALL) {

			/* if extracting all of a water year, need to */
			/* check month and year */

			if (rmonth == XTRACT_ALL && hydro_year) {
				if (month >= 10 && year != ryear - 1)
					do_xtract = FALSE;
				else if (month < 10 && year != ryear)
					do_xtract = FALSE;

			} else if (ryear != year) {
				do_xtract = FALSE;
			}
		}
		if (rmonth != XTRACT_ALL && rmonth != month) {
			do_xtract = FALSE;
		}

		/* extract data if this a requested day */

		if (do_xtract) {

			start_day = (rec_id - 1) * 8 + 1;
			end_day = start_day + 8;

			if (rday == XTRACT_ALL ||
			    ( rday >= start_day && rday <= end_day) ) {

				xtract_data (&line[DATA], station_ID, type_code,
					     year, month, start_day, end_day,
					     hydro_year, rday, runoff,
					     drain_area,
					     got_loc, easting, northing);
			}
		}
	}
}
