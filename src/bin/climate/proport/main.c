#include "ipw.h"
#include "pgm.h"

int
main (
     int             argc,
     char          **argv)
{
    static OPTION_T opt_y = {
        'y', "w = water-year; c = calendar-year",
        STR_OPTARGS, "yeartype",
        REQUIRED, 1, 1
      };
    
    static OPTION_T opt_o = {
        'o', "output file prefix (suffix is .1 to .12)",
        STR_OPTARGS, "out_prefix",
        REQUIRED, 1, 1
      };

    static OPTION_T operand = {
        OPERAND, "ASCII input file of station data",
        STR_OPERANDS, "infile",
        OPTIONAL, 1, 1
      };
    
    static OPTION_T *optv[] = {
        &opt_y,
        &opt_o,
        &operand,
        0
      };
    
    FILE   *fdi;          /* input station data file */
    char   *year;         /* yeartype */
    char   *outprefix;    /* output file prefix */
    char    buf[10];
    
    /* begin */
    
    ipwenter (argc, argv, optv, IPW_DESCRIPTION);
    
    /* process options */

    year = buf;
    
    year = str_arg(opt_y, 0);
    if (year[0] != 'w' && year[0] != 'c') {
        error ("yeartype must be 'w' or 'c'.");
    }

    outprefix = str_arg(opt_o, 0);

    if (!got_opt(operand)) {
        usage();
    }
    else
      {
          fdi = fopen(str_opnd(operand, 0), "r");
          if (fdi == NULL)
            error("Can't open \"%s\"", str_opnd(operand, 0));
      }

    /* do it */
    
    proport (year[0], fdi, outprefix);
    
    /* all done */
    
    fclose (fdi);
    ipwexit (EXIT_SUCCESS);
}

