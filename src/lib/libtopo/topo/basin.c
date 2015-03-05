#include <math.h>

#include "ipw.h"
#include "topo.h"
#include "_cellstack.h"
#include "_neighbor.h"

fpixel_t
basin(
       int	  seedX,	/* sample number of seed point		*/
       int	  seedY,	/* line number of seed point		*/
       fpixel_t	* slope,	/* array of sin(slope) data for pixels	*/
       fpixel_t	* exposure,	/* array of exposure data for pixels	*/
       fpixel_t	* elev,		/* array of elevation data for pixels	*/
       pixel_t	* bmask,	/* output basin mask			*/
       int	  x_count,	/* number of samples in image		*/
       int	  y_count,	/* number of lines in image		*/
       fpixel_t	  flat,		/* threshold for "flat" slopes		*/
       int	  clipFlag,	/* clip basin to a specific altitude?	*/
       fpixel_t	  clipElev)	/* elevation at which to clip the basin	*/
{
   fpixel_t boundaryElev; /* Lowest boundary on the perimeter (result)      */
   int	    boundaryFlag; /* True if current cell is on the basin boundary  */
   unsigned int code;	  /* Bit mask of cur. cell's valid neighbors        */
   unsigned int upstream; /* Bit mask of cur. cell's upstream neighbors     */
   int 	    n_index;      /* Array index of neighbor under consideration    */
   int	    neighbor;     /* Neighbor code of neighbor under consideration  */ 
   int      dx, dy;       /* 2D grid offset of neighbor under consideration */
   CELL_STACK *stack;	  /* Recursion stack                                */
   struct 
      { 
      fpixel_t 		threshold;
      unsigned int	neighbors;
      } octant[8];	  /* Lookup table for upstream cells                */

   static bool_t init_neighbor = FALSE;


   assert( seedX >= 0 && seedY >= 0 && seedX < x_count && seedY < y_count );

   if ( clipFlag && elev == NULL )
      {
      error("basin():  Must supply elevation data for elevation clipping.\n");
      }

   /* 
      Initialize stack, leave enough space for each cell to be
      put on the stack by each of its eight neighbors
      */
   init_stack( &stack, x_count * y_count * 8) ;

   /*
      Initialize neighbor data structure.
      */
   if (! init_neighbor)
      {
      init_neighbor = TRUE;
      neighbor_init( x_count );
      }

   /* 
      Create the exposure direction lookup table.  This table partitions
      the exposures into 8 octants.  Each octant indicates two neighbors
      that are to be considered upstream.  This lookup table strategy is 
      less efficient, but more portable than the original QDIPS version of
      the function, which used bit arithmetic.
      
      Exposures are measured in radians from -Pi to Pi.  An exposure of 0.0 
      is considered south-facing.
      */
   octant[0].threshold = -.75 * M_PI ;
   octant[0].neighbors = (1 << NEIGHBOR_S) | (1 << NEIGHBOR_SE);
   
   octant[1].threshold = -.50 * M_PI ;
   octant[1].neighbors = (1 << NEIGHBOR_SE) | (1 << NEIGHBOR_E);
   
   octant[2].threshold = -.25 * M_PI ;
   octant[2].neighbors = (1 << NEIGHBOR_E) | (1 << NEIGHBOR_NE);
   
   octant[3].threshold = 0.0 ;
   octant[3].neighbors = (1 << NEIGHBOR_NE) | (1 << NEIGHBOR_N);
   
   octant[4].threshold = .25 * M_PI ;
   octant[4].neighbors = (1 << NEIGHBOR_N) | (1 << NEIGHBOR_NW);
   
   octant[5].threshold = .50 * M_PI ;
   octant[5].neighbors = (1 << NEIGHBOR_NW) | (1 << NEIGHBOR_W);
   
   octant[6].threshold = .75 * M_PI ;
   octant[6].neighbors = (1 << NEIGHBOR_W) | (1 << NEIGHBOR_SW);
   
   octant[7].threshold = M_PI ;
   octant[7].neighbors = (1 << NEIGHBOR_SW) | (1 << NEIGHBOR_S);

   /* 
      Initialize lowest point on the boundary.
      */
   if ( elev != NULL )
      boundaryElev = FPIXEL_MAX ;
   else
      boundaryElev = 0.0 ;

   /* 
      Push the seed point onto the stack 
      */
   push_stack( stack, seedX, seedY, seedY * x_count + seedX );
   
   /* Begin loop */
   while ( !is_empty( stack ) )
      {
      int i;
      int index;
      int x;
      int y;
      
      /* Get the next element off the stack */
      assert( pop_stack( stack, &x, &y, &index ) == 0 );
      
      /* Nothing to do if this cell was already visitted */
      if ( bmask[index] != 0 ) 
	 {
	 assert(bmask[index] == 255 );
	 continue;
	 }
      
      /* Otherwise, mark this grid cell and continue upstream */
      bmask[index] = 255;

      /* Figure out which neighbors are valid grid cells */
      code = ~0 ;
      if ( x == 0 )         code &= ~Neighbor.west  ;
      if ( y == 0 )         code &= ~Neighbor.north ;
      if ( x == x_count-1 ) code &= ~Neighbor.east  ;
      if ( y == y_count-1 ) code &= ~Neighbor.south ;
      
      /* 
	 Based on exposure, figure out which neighbors are upstream.
	 After this code segment, "upstream" is a bit mask that indicates
	 those neighbor codes that are upstream. 
	 */
      upstream = 0;
      for ( i=0; i < 7; i++ )
	 {
	 if (exposure[index] <= octant[i].threshold)
	    {
	    upstream = octant[i].neighbors;
	    break;
	    }
	 }
      if ( upstream == 0 )
	 upstream = octant[7].neighbors ;

      /* Start out by assuming this cell is not on the boundary */
      boundaryFlag = FALSE ;

      /* For each 8-neighbor....*/
      for ( neighbor=0; neighbor < 8; neighbor++ )
	 {
	 /* Don't persue if this neighbor is not valid */
	 if ( ! ( code & ( 1 << neighbor ) ) )
	    continue;
	 
	 /* Find array index of this neighbor */
	 n_index = index + Neighbor.offset[neighbor];
	  
	 /* Skip if this neighbor already visitted */
	 if ( bmask[n_index] )
	    {
	    boundaryFlag = TRUE ;
	    continue;
	    }

	 /* 
	    Skip if we are clipping based on elevation and this neighbor is 
	    too high.
	    */
	 if ( clipFlag && elev != NULL )
	    {
	    if ( elev[n_index] > clipElev )
	       {
	       boundaryFlag = TRUE;
	       continue;
	       }
	    }

	 dx = Neighbor.offset_x[ neighbor ];
	 dy = Neighbor.offset_y[ neighbor ];
	 
	 /* Push this neighbor if it is nearly flat */
	 if ( slope[n_index] <= flat )
	    {
	    push_stack( stack, x+dx, y+dy, n_index );
	    continue;
	    }
	 
	 /* Push this neighbor if it is upstream */
	 if ( upstream & ( 1 << neighbor ) )
	    {
	    push_stack( stack, x+dx, y+dy, n_index );
	    continue;
	    }

	 /* 
	    If we arrive here, then we have a neighbor that is on the 
	    boundary.  Update the lowest boundary variable
	    */
	 boundaryFlag = TRUE ;
	 } /* End neighbors loop */
      
      if ( boundaryFlag && ( elev != NULL ) )
	    {
	    if ( elev[index] < boundaryElev )
	       boundaryElev = elev[index];
	    }
      } /* End while stack not empty loop */

   dealloc_stack( stack );

   return( boundaryElev );
}
