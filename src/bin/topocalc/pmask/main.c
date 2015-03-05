#include <strings.h>

#include "ipw.h"
#include "pgm.h"
#include "topo.h"

int 		_x_count;
int 		_y_count;
fpixel_t	*_elev;
fpixel_t	*_slope;
fpixel_t	*_exposure;
pixel_t		*_pmask;

int
main(
	int     argc,
	char ** argv)
{
    static OPTION_T opt_t = 
	{
	't', "Threshold elevation difference to be considered uphill",
	REAL_OPTARGS, "thresh",
	OPTIONAL, 1, 1
	};

    static OPTION_T opt_s = 
	{
	's', "Only output pit seed points.",
	OPTIONAL, 0, 0
	};

    static OPTION_T opt_f = 
	{
	'f', "Tolerance for zero slope",
	REAL_OPTARGS, "float",
	OPTIONAL, 1, 1
	};

#if 0
    static OPTION_T opt_i = 
	{
	'i', "invert the results",
	OPTIONAL, 0, 0
	};
#endif

    static OPTION_T operands = 
	{
	OPERAND, "input 3-band image (elev, gradient file)",
	STR_OPERANDS, "elev + gradient",
	OPTIONAL, 1, 1
	};
  
    static OPTION_T *optv[] = 
	{
	&opt_t,
	&opt_s,
	&opt_f,
#if 0
	&opt_i,
#endif
	&operands,
	0
	};

    int	fdi;			/* input  file descriptor	 */
    int fdo;			/* output file descriptor  	*/
    
    int i, j, di, dj;
    int found, index, n_index;
    fpixel_t flat, thresh;
    int	seedsOnly;
    pixel_t *tempmask;
    fpixel_t boundary_elev;
    

    /*
     * begin
     */
    ipwenter( argc, argv, optv, IPW_DESCRIPTION);
    
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

    if ( got_opt( opt_t ) )
	thresh = real_arg( opt_t, 0 );
    else
	thresh = 0;

    if ( got_opt( opt_f ) )
	flat = real_arg( opt_f, 0 );
    else
	flat = .001;

    fprintf(stderr,"Flat is %f.\n", flat);

    if ( got_opt( opt_s ) )
	seedsOnly = TRUE;
    else
	seedsOnly = FALSE;
    /*
     * do it
     */
    fprintf( stderr, "Reading input file..." ); fflush( stderr );
    fileRead ( fdi );
    fprintf( stderr, "Done.\n");

    /* Allocate space for temporary pit mask, if necessary */
    if ( seedsOnly == FALSE )
	{
	tempmask    = ( pixel_t  * ) ecalloc ( _x_count * _y_count, sizeof( pixel_t  ) );
	assert( tempmask != NULL );
	}
    else
       tempmask = NULL ;

    fprintf( stderr, "Finding pits..." ); fflush( stderr );
    for ( j=1; j < _y_count-1 ; j++ )
	for ( i=1; i < _x_count-1 ; i++ )
	    {
	    index = j * _x_count + i;
	    n_index = index - _x_count - 1;
	    found = FALSE;
	    
	    /* Skip this cell if it's already marked as being in a pit */
	    if ( _pmask[index] == 255 )
	       continue;

	    /* Look for an downhill neighbor */
	    for ( dj = -1; dj <=1; dj++ )
		{
		for (di = -1; di <=1; di++ )
		    {
		    if (( di || dj ) && 
			( _elev[ n_index ] - _elev[ index ]  < thresh ))
			{
			found = TRUE;
			break;
			}
			
		    n_index++;
		    }
		
		/* Break out of dj loop if we found a downhill neighbor */
		if (found) break; 
		n_index += _x_count-3;
		}

	    /* If no downhill neighbors, this is a pit seed point */
	    if ( !found )
		{
		if ( seedsOnly )
		  {
		  /* We're only supposed to mark the seed points. */
		  _pmask[index] = 255;
		  }
		else
		  {
		  /* Find lowest boundary elevation for this pit */
		  /* Use temporary mask buffer; we'll throw away the result */
		  boundary_elev = basin( i, j, _slope, _exposure, _elev, 
					tempmask, _x_count, _y_count, 
					flat, FALSE, (fpixel_t) 0.0 );

		  bzero( tempmask, _x_count * _y_count * sizeof( pixel_t ) );

		  /* Mark this pit by clipping its basin to lowest boundary
		     elevation
		     */
		  boundary_elev = basin( i, j, _slope, _exposure, _elev, 
					_pmask, _x_count, _y_count, 
					flat, TRUE, boundary_elev );
		  }
		}
	    
	    }

    fprintf( stderr, "Done.\n");

#if 0
    if ( got_opt( opt_i ) )
	{
	fprintf(stderr,"Inverting result....");
	for (i=0; i < _x_count * _y_count; i++)
	    if ( _pmask[i] )
		_pmask[i] = 0;
	    else
		_pmask[i] = 255;
	
	fprintf(stderr,"Done.\n");
	}
#endif

    fprintf( stderr, "Writing output file.." ); fflush( stderr );
    fileWrite( fdo );
    fprintf( stderr, "Done.\n");
    
    /*
     * end
     */
    ipwexit(EXIT_SUCCESS);
}
