/*
** NAME
**      open_files - opens input and output files
**
** SYNOPSIS
**      #include "pgm.h"
**
**      void
**	open_files(void)
**
** DESCRIPTION
**      Opens input and output files for 2-layer snowmelt model snobal.
**
** GLOBAL VARIABLES READ
**	i
** GLOBAL VARIABLES MODIFIED
**
*/

#include "ipw.h"
#include "pgm.h"

void
open_files(void)

{
/***    open snow properties input file ***/

        if (sn_file) {
		if ((snf = fopen(sn_filename, "r")) == NULL) {
                	error("Error opening snow properties file: %s",
				sn_filename);
		}
	}

/***    open measurement heights input file ***/

        if (mh_file) {
		if ((mhf = fopen(mh_filename, "r")) == NULL) {
                	error("Error opening measurement heights file: %s",
				mh_filename);
		}
	}

/***    open precipitation input file ***/

        if (pr_file) {
		if ((prf = fopen(pr_filename, "r")) == NULL) {
                	error("Error opening precipitation file: %s",
				pr_filename);
		}
	}

/***    open input file ***/

        if (in_file) {
		if ((in = fopen(in_filename, "r")) == NULL) {
                	error("Error opening input file: %s", in_filename);
		}
	}

/***    open output file        ***/

        if (out_file) {
		if ((out = fopen(out_filename, "w")) == NULL) {
                	error("Error opening output file: %s", out_filename);
		}
	}
}
