#include "ipw.h"
#include "pgm.h"
#include "topo.h"

int 		_x_count;   /* The width of the grid (number of samples)    */
int 		_y_count;   /* The height of the grid (number of lines)     */
fpixel_t	*_slope;    /* The array of slope data                      */
fpixel_t	*_exposure; /* The array of exposure data                   */
pixel_t		*_bmask;    /* The basin mask that is being created         */

int
main(
	int     argc,
	char ** argv)
{
    static OPTION_T opt_p = 
	{
	'p', "line, sample coordinates of the grid point of interest",
	INT_OPTARGS, "coord",
	REQUIRED, 2, 2
	};

    static OPTION_T opt_m = 
	{
	'm', "pit_mask file",
	STR_OPTARGS, "mask",
	OPTIONAL, 1, 1
	};

    static OPTION_T opt_f = 
	{
	'f', "Tolerance for zero slope",
	REAL_OPTARGS, "float",
	OPTIONAL, 1, 1
	};

    static OPTION_T operands = 
	{
	OPERAND, "input gradient file",
	STR_OPERANDS, "image",
	OPTIONAL, 1, 1
	};
  
    static OPTION_T *optv[] = 
	{
	&opt_p,
	&opt_f,
	&opt_m,
	&operands,
	0
	};

    int	fdi;			/* input  file descriptor	*/
    int fdo;			/* output file descriptor  	*/
    int fdm;			/* file descriptor for pit mask */
    
    int x, y, *pargs;
    float ignore;
    fpixel_t flat;

    /*
     * begin
     */
    ipwenter(argc, argv, optv, IPW_DESCRIPTION);
    
    /*
     * access input and output files 
     */
    if (!got_opt(operands)) 
	{
	fdi = ustdin();
	}
    else 
	{
	fdi = uropen(str_arg(operands, 0));
	if (fdi == ERROR) 
	    error("can't open \"%s\"", str_arg(operands, 0));
	}
    no_tty(fdi);

    fdo = ustdout();
    no_tty(fdo);

    pargs = int_argp( opt_p );
    x = pargs[0];
    y = pargs[1];

    if ( got_opt( opt_f ) )
	flat = real_arg( opt_f, 0 );
    else
	flat = .001;

    fprintf(stderr,"Flat is %f.\n", flat);

    if (got_opt( opt_m )) 
	{
	fdm = uropen(str_arg(opt_m, 0));
	if (fdm == ERROR) 
	    error("can't open \"%s\"", str_arg(operands, 0));
	no_tty(fdm);
	}
    else
	fdm = -1;

    /*
     * do it
     */
    fprintf( stderr, "Reading input file..." ); fflush( stderr );
    fileRead ( fdi, fdm );
    fprintf( stderr, "Done.\n");

    fprintf( stderr, "Running masking algorithm..." ); fflush( stderr );
    ignore = basin( x, y, _slope, _exposure, NULL, _bmask, 
		    _x_count, _y_count, flat, 0, (fpixel_t) 0.0 );
    fprintf( stderr, "Done.\n");

    fprintf( stderr, "Writing output file.." ); fflush( stderr );
    fileWrite( fdo );
    fprintf( stderr, "Done.\n");
    
    /*
     * end
     */
    ipwexit(EXIT_SUCCESS);
}
