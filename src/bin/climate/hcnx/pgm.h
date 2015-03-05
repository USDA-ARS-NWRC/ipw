#ifndef PGM_H
#define PGM_H

#include	"IPWmacros.h"

/* legal data type codes */

#define NTYPES	4
#define	PRCP	0
#define TMIN	1
#define TMAX	2
#define ROFF	3

#define XTRACT_ALL	-1
#define MIN_YEAR	1948
#define MAX_YEAR	1988

#define MAXLINE		85
#define DATA		24		/* index of start of data in line*/
#define MM_PER_INCH	25.4		/* conversion inches to mm	 */
#define SEC_PER_DAY	86400		/* seconds per day		 */
#define IN_PER_FT	12		/* inches per foot		 */
#define FT_PER_MILE	5280		/* feet per mile		 */

/* conversion of runoff in cubic feet/sec divided by drainage area in */
/* square miles to millimeters per square meter per day */
/* (SEC_PER_DAY * IN_PER_FEET / (FT_PER_MILE * FT_PER_MILE) * MM_PER_INCH) */

#define ROFF_CFACTOR	0.944628
#define R_NODATA	999999		/* data flag for no runoff data	 */
#define BLANK		' '

extern char *types[];
extern char *types_lc[];

extern void	hcnx(FILE *ifp, int ryear, int rmonth, int rday,
		     bool_t hydro_year, bool_t *xtract, bool_t runoff,
		     float drain_area, bool_t got_loc, double easting,
		     double northing);
extern void	output_dat(int type_code, int year, int yd, char *station_ID,
			   char sus_val, int data_code, int data, bool_t runoff,
			   float drain_area, bool_t got_loc, double easting,
			   double northing);
extern void	xtract_data(char *line, char *station_ID, int type_code,
			    int year, int month, int start_day, int end_day,
			    bool_t hydro_year, int rday, bool_t runoff,
			    float drain_area, bool_t got_loc, double easting,
			    double northing);

#endif
