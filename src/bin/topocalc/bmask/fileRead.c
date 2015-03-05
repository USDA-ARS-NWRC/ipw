#include "ipw.h"
#include "gethdrs.h"
#include "bih.h"
#include "pixio.h"
#include "fpio.h"
#include "lqh.h"
#include "pgm.h"

void
fileRead(
       int	i_fd,
       int	pit_fd)
{
    /* Image header variables */
    BIH_T       **i_bihpp;	/* Basic Image Header -- Input */
    int		size;		/* total array size (in pixels)  */

    /* Buffers and buffer pointers */
    fpixel_t 	*io_buf;
    fpixel_t	*io_ptr;	
    fpixel_t	*slope_ptr;	
    fpixel_t	*exposure_ptr;	
    pixel_t	*mask_buf;
    pixel_t	*mask_ptr;

    /* Loop counters */
    int		i, j;

    /* Variable for copying header from input to output */
    static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread };
    static GETHDR_T *hv[] = { &h_lqh, NULL };
    static GETHDR_T *pitv[] = {NULL};

    /* Ingest input image header*/
    i_bihpp = bihread( i_fd );
    assert( i_bihpp != NULL );
    assert( bih_nbands( i_bihpp[0] ) == 2 );

    _x_count = bih_nsamps( i_bihpp[0] );
    _y_count = bih_nlines( i_bihpp[0] );
    size = _x_count * _y_count;

    /* Ingest remaining headers */
    gethdrs( i_fd, hv, NO_COPY, -1 );

    /* Allocate buffers for dem, and output buffers */
    _slope    = ( fpixel_t * ) ecalloc ( size, sizeof( fpixel_t ) );
    _exposure = ( fpixel_t * ) ecalloc ( size, sizeof( fpixel_t ) );
    _bmask    = ( pixel_t  * ) ecalloc ( size, sizeof( pixel_t  ) );
    assert( _slope != NULL && _exposure != NULL && _bmask != NULL );
    
    /* Read in gradient data */
    slope_ptr    = _slope;
    exposure_ptr = _exposure;
    io_buf = ( fpixel_t * ) ecalloc ( _x_count * 2, sizeof ( fpixel_t ) );
    assert( io_buf != NULL );
    for ( j=0; j < _y_count; j++ )
	{
	io_ptr = io_buf;
	if ( fpvread( i_fd, io_ptr, _x_count) != _x_count ) 
	    {
	    error ("read error, line %d", j);
	    }
	for ( i=0; i < _x_count; i++ )
	    {
	    *slope_ptr++     = *io_ptr++;
	    *exposure_ptr++  = *io_ptr++;
	    }
	}

    /* Read in and process pit mask, if necessary */
    if ( pit_fd >= 0 )
	{
	i_bihpp = bihread( pit_fd );
	assert( i_bihpp != NULL );
	assert( bih_nbands( i_bihpp[0] ) == 1 );
	assert( _x_count == bih_nsamps( i_bihpp[0] ) );
	assert( _y_count == bih_nlines( i_bihpp[0] ) );
	
	/* Ingest remaining headers */
	gethdrs( pit_fd, pitv, NO_COPY, -1 );

	mask_buf = ( pixel_t * ) ecalloc( _x_count, sizeof( pixel_t) );
	assert( mask_buf != NULL );
	
	/* Read mask data and mask slope file */
	slope_ptr = _slope;
	for ( j=0; j < _y_count; j++ )
	    {
	    mask_ptr = mask_buf;
	    if ( pvread( pit_fd, mask_ptr, _x_count) != _x_count ) 
		{
		error ("read error, line %d", j);
		}
	    for ( i=0; i < _x_count; i++ )
		{
		if ( *mask_ptr++ != 0 )
		    *slope_ptr++ = 0.0;
		else
		    slope_ptr++;
		}
	    }
	}

}
