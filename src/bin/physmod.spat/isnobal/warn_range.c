/*
 ** NAME
 **      check_range - checks if value within a specified range
 **
 ** SYNOPSIS
 **      void
 **	check_range(
 **	    double  value,		|* value to check *|
 **	    double  min,		|* minimum acceptable value *|
 **	    double  max,		|* maximum acceptable value *|
 **	    char   *descrip,		|* description of value being checked *|
 **	    bool_t  print_line_samp)	|* print line and sample in message? *|
 **
 ** DESCRIPTION
 **	This routine checks if the value is within a specified range.
 **	<descrip> contains a description of the value being checked.  The
 **	range is between <min> and <max>.  If the value is out of range,
 **	prints an error message.  If the flag 'print_line_samp' is TRUE,
 **	the error message includes the current line # and sample #.
 **
 **	Note: This routine does not return if the value is out of range.
 **
 ** GLOBAL VARIABLES READ
 **	line
 **	samp
 **
 ** HISTORY
 **	Jun 96	Developed from the 'check_fval' routine of 'snobal'.
 **		J. Domingo, OSU
 */

#include <math.h>
#include "ipw.h"
#include "pgm.h"

void
warn_range(
		int		index,		 /* index value */
		double	value,		 /* value to check */
		double	min,		 /* minimum acceptable value */
		double	max,		 /* maximum acceptable value */
		char *	descrip,	 /* description of value being checked */
		bool_t  print_line_samp) /* print line and sample in message? */
{
	int line;
	int samp;

	if (value < min || value > max) {
		if (print_line_samp) {

			line = (int) floor((double) index / nsamps);
			samp = index - (line*nsamps) - 1;

			warn("At index %d, line %d, sample %d, %s (%f) out of range: %f to %f",
					index, line, samp, descrip, value, min, max);
		}
		else {
			warn("%s (%f) out of range: %f to %f", descrip, value,
					min, max);
		}
	}
}
