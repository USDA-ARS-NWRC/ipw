#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "lqh.h"
#include "pgm.h"

void
fileWrite(
       int	o_fd)
{
    BIH_T       **o_bihpp;	/* Basic Image Header -- Output */
    pixel_t	*io_ptr;	/* Current location in output buffer */
    int		j;

    /* Make bih for output */
    o_bihpp  = ( BIH_T ** ) ecalloc ( 1, sizeof( BIH_T * ) );
    assert( o_bihpp != NULL );    
    o_bihpp[0] = bihmake( 1, 8, NULL, NULL, NULL,  _y_count, _x_count, 1);
    assert( o_bihpp[0] != NULL );
    assert( bihwrite(o_fd, o_bihpp) != ERROR);
    assert( boimage( o_fd ) != ERROR);

    /* Write output file */
    io_ptr = _pmask;
    for ( j=0; j < _y_count; j++)
	{
	if ( pvwrite( o_fd, io_ptr, _x_count) != _x_count ) 
	    {
	    error ("write error, line %d", j);
	    }

	io_ptr += _x_count;
	}
}





