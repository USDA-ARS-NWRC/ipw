/*
** NAME
**      pgm.h
**
** DESCRIPTION
**      The include file for 'snobal'.
**
** HISTORY
**	Feb 1996	Created from the old set_con.c file.  J. Domingo, OSU
*/

#ifndef _SNOBAL_PGM_H_
#define _SNOBAL_PGM_H_

#include "IPWmacros.h"
#include "snobal.h"

/* ------------------------------------------------------------------------- */

/*
 *  Routines that are part of snobal program.
 */

extern void	check_range (double value, double min, double max,
			     char * descrip);
extern int      get_in_rec  (INPUT_REC * in_rec);
extern void     get_mh_rec  (void);
extern void     get_pr_rec  (void);
extern void     get_sn_rec  (void);
extern void     get_args    (int argc, char **argv);
extern double	getf        (char *prompt, double minval, double maxval);
extern int	getok       (char *prompt);
extern void     initialize  (void);
extern void     open_files  (void);
extern void     output	    (void);
extern void     sn_err	    (double new_z_s, double new_rho, double new_T_s_0,
			     double new_T_s, double new_h2o_sat);

/* ------------------------------------------------------------------------- */

/*
 *  Global variables internal to snobal program.
 */

/*   input constants            */

extern	double  elevation;      /* elevation (m) */


/*   snow-conditions update file */

extern	int	sn_file;	/* snow properties records in file? */
extern	char   *sn_filename;   /* snow properties input file name */
extern	FILE   *snf;		/* snow properties input file ptr */
extern	int     more_sn_recs;   /* any more snow-properties records left? */
extern	double  time_s;         /* time of next snow-properties record (secs) */

/*   input parameter file */

extern	int     in_file;        /* input records in file? */
extern	char   *in_filename;	/* input file name */
extern	FILE   *in;		/* input file ptr */

/*   measurement heights file	*/

extern	int	mh_file;	/* measurement-heights records in file? */
extern	char   *mh_filename;	/* measurement heights input file name */
extern	FILE   *mhf;		/* measurement heights input file ptr */
extern	int     more_mh_recs;   /* any more meas-heights records left? */
extern	double  time_z;         /* time of next measurement-ht record (secs) */

/*   precipitation file		*/

extern	int	precip_data;	/* precipitation data? */
extern	int	pr_file;	/* precipitation records in file? */
extern	char   *pr_filename;	/* precipitation input file name */
extern	FILE   *prf;		/* precipitation input file ptr */
extern	int     more_pr_recs;   /* any more precip records left? */
extern	double  time_pp;        /* time of next precipitation record (secs) */

/*   output file  		*/

extern	int     out_file;       /* output file specified? */
extern	char   *out_filename;	/* output file name */
extern	FILE   *out;		/* output file ptr */

/*   work vars                  */

extern	int	temps_in_C;	/* temperatures in degrees C? */
extern	double  start_time;     /* start time (sec) */
extern	int     rec_count;      /* record counter */
extern	double  curr_time_hrs;  /* time of current time step (hrs) */

#endif /* _SNOBAL_PGM_H_ */
