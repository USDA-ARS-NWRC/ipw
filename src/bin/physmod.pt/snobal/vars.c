/*
** NAME
**      vars.c
**
** DESCRIPTION
**      defines and allocates space for variables declared
**      in "extern.h";
**
** HISTORY
**      July, 1985:  	written by D. Marks, CSL, UCSB;
**	January, 1990:  added variables necessary for new input files
**			by Kelly Longley, OSU;
**	May 1995	Converted to IPW by J.Domingo, OSU
**	Feb 1996	Changed variable names to match notation in
**			snowmelt papers (e.g. Marks and Dozier, 1992).
**			J. Domingo, OSU
*/

#include        "ipw.h"
#include        "pgm.h"



/*   input constants            */

        double  elevation;      /* elevation (m) */


/*   snow-conditions update file */

	int	sn_file;	/* snow properties records in file? */
        char   *sn_filename;	/* snow properties input file name */
        FILE   *snf;		/* snow properties input file ptr */
        int     more_sn_recs;   /* any more snow-properties records left? */
        double  time_s;         /* time of next snow-properties record (secs) */

/*   input parameter file */

        int     in_file;        /* input records in file? */
        char   *in_filename;    /* input file name */
        FILE   *in;		/* input file ptr */

/*   measurement heights file	*/

	int	mh_file;	/* measurement-heights records in file? */
        char   *mh_filename;	/* measurement heights input file name */
        FILE   *mhf;		/* measurement heights input file ptr */
        int     more_mh_recs;   /* any more meas-heights records left? */
        double  time_z;         /* time of next measurement-ht record (secs) */

/*   precipitation file		*/

	int	precip_data;	/* precipitation data? */
	int	pr_file;	/* precipitation records in file? */
        char   *pr_filename;	/* precipitation input file name */
        FILE   *prf;		/* precipitation input file ptr */
        int     more_pr_recs;   /* any more precip records left? */
	double  time_pp;        /* time of next precipitation record (secs) */

/*   output file  		*/

        int     out_file;       /* output file specified? */
        char   *out_filename;	/* output file name */
        FILE   *out;		/* output file ptr */

/*   work vars                  */

	int	temps_in_C;	/* temperatures in degrees C? */
        double  start_time;     /* start time (sec) */
        int     rec_count;      /* record counter */
        double  curr_time_hrs;  /* time of current time step (hrs) */
