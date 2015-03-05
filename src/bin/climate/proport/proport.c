#include "ipw.h"
#include "pgm.h"

#define NMONTHS 12
#define WARN_THRESH 15 /* print a warning if > WARN_THRESH
			nodata values for the current month */
#define NODATA  -9999.0  /* output value for nodata (negative) observations */
  
void proport(
	char    yeartype,
	FILE    *fdi,
	char    *outprefix)
{
    int     i;  
    int     nread;
    int	    nodata;  		/* counter for monthly nodata values */
    int	    tonodata   = 0; 	/* counter for total nodata values */
    int     stat_count = 0;     /* counter for met station-months */
    int     cur_day;		/* current day of year (0-364) */
    int     day_index;
    int     month_index;
    double  pyear[365], precip[31];
    double  month_sum;
    FILE    **fdo;
    char    tmp_name[15];
    char    id[100], northing[100], easting[100];
    int    *day_count;
    static int   cal_year[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    static int water_year[] = {31,30,31,31,28,31,30,31,30,31,31,30};
    
    if (yeartype == 'w') {
	printf("\nAssuming data are in water-year order...\n");
	day_count = water_year;
    }
    else {
        printf("\nAssuming data are in calendar-year order...\n");
	day_count = cal_year;
    }
    
    if ((fdo = (FILE **)ecalloc (NMONTHS, sizeof(tmp_name))) == NULL) {
          error ("Could not allocate memory for output file array.");
    }
    
    for (month_index = 0; month_index < NMONTHS; month_index++) {
          (void)sprintf (tmp_name, "%s.%d", outprefix, month_index + 1);
          fdo[month_index] = fopen (tmp_name, "w");
	  if (fdo[month_index] == NULL) error("Couldn't open the file \"%s\"\n",
		tmp_name); 
    }

    nread = 1;
    
    while (nread > 0) {

    	/* read a record from the met station data */

    	nread = fscanf(fdi, "%s %*s %s %s %*s", id, northing, easting );
	if (nread < 0) break;
	if (nread != 3) 
	    error ("Premature end of input file.");

	for (i = 0; i < 365; i++) {
	    nread = fscanf(fdi, "%lf", &pyear[i]);
	    if (nread != 1) 
		error ("Premature end of input file.");
	}
          
	cur_day = 0;

        /* for each month */

        for (month_index = 0; month_index < NMONTHS; month_index++) {
            month_sum = 0.0;
	    nodata = 0;

  	    /* Put current month's precip into the precip[] array 
	       and set negative values to NODATA */

            for (day_index=0; day_index < day_count[month_index]; day_index++){

		precip[day_index] = pyear[cur_day++];

                if (precip[day_index] < 0.0) {
		    nodata++;
		    tonodata++;
                } else {
                    month_sum += precip[day_index];
	        }
            }

	    stat_count++;

	    /* warn about missing values */

	    if (nodata >= WARN_THRESH) {
	      fprintf(stderr, 
		"NOTE: station '%s' for month %d: %d nodata values.\n",
		id, month_index + 1, nodata);
	    }

	    /* write data to file */

            fprintf (fdo[month_index], "%s %s ", easting, northing);
            for (day_index=0; day_index < day_count[month_index]; day_index++)
            {
                if (month_sum == 0.0) {
                    fprintf (fdo[month_index], "%.1f ", 0.0);
                }
                else {
		    if (precip[day_index] < 0.0) 
			fprintf (fdo[month_index], "%.0f ", NODATA);
		    else
                        fprintf (fdo[month_index], "%.6f ", 
			    precip[day_index] / month_sum);
                }
            }
            fprintf (fdo[month_index], " \n");
	
        } /* end month loop */
    }     /* end record loop */
    
    for (month_index = 0; month_index < NMONTHS; month_index++) {
          fclose (fdo[month_index]);
    }

    printf("\n");
    printf("Total stations:            %d\n", stat_count / NMONTHS);
    printf("Total station-days:        %d\n", stat_count / NMONTHS * 365); 
    printf("Total missing values:      %d\n", tonodata); 
    printf("%% missing values:          %.2f\n\n", 
	(float) tonodata / (stat_count / NMONTHS * 365.0) * 100.0); 

} /* end program */

