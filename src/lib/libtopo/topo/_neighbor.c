#include <stdio.h>
#include <math.h>

#include "ipw.h"
#include "_neighbor.h"

NEIGHBOR_INFO Neighbor;

void
neighbor_init(
       int x_count)
{
   /* 
      This function initializes the global "Neighbor" variable that contains
      information about the encoding of a cells' 8-neighbors.  In order to 
      perform the mapping, it is necessary to know the width of the grid 
      (number of samples per row).
      
      It's kind of ugly to have a global floating around, but dynamic 
      calculation of some of these fields allows us to specify the neighbor
      codes in neighbor.h and have everything else derived from that.
      */
   int i;

   /* 
      Step 1:  Set up offset table

      The grid is stored as a one-dimensional array.  The offset table gives
      the change in cell address as we move from a cell to its neighbor.
      */
   Neighbor.offset_x[ NEIGHBOR_NW ] = -1 ;
   Neighbor.offset_x[ NEIGHBOR_N  ] =  0 ;
   Neighbor.offset_x[ NEIGHBOR_NE ] =  1 ;
   Neighbor.offset_x[ NEIGHBOR_W  ] = -1 ;
   Neighbor.offset_x[ NEIGHBOR_E  ] =  1 ;
   Neighbor.offset_x[ NEIGHBOR_SW ] = -1 ;
   Neighbor.offset_x[ NEIGHBOR_S  ] =  0 ;
   Neighbor.offset_x[ NEIGHBOR_SE ] =  1 ;

   Neighbor.offset_y[ NEIGHBOR_NW ] = -1 ;
   Neighbor.offset_y[ NEIGHBOR_N  ] = -1 ;
   Neighbor.offset_y[ NEIGHBOR_NE ] = -1 ;
   Neighbor.offset_y[ NEIGHBOR_W  ] =  0 ;
   Neighbor.offset_y[ NEIGHBOR_E  ] =  0 ;
   Neighbor.offset_y[ NEIGHBOR_SW ] =  1 ;
   Neighbor.offset_y[ NEIGHBOR_S  ] =  1 ;
   Neighbor.offset_y[ NEIGHBOR_SE ] =  1 ;

   for ( i=0; i < 8 ; i++ )
      Neighbor.offset[ i ] = x_count * Neighbor.offset_y[ i ] 
	 + Neighbor.offset_x[ i ] ;

   /*
      Step 2:  Set up boundary masks

      At the edges of the grid, we want to be able to quickly eliminate
      those neighbors that lie outside of the grid.  The boundary masks
      are based on a bitmask representation of the neighbors.  For example,
      neighbor number 3 is represented by the 3rd bit from the right in a 
      bit mask.
      */

   Neighbor.north = (1<<NEIGHBOR_N) | (1<<NEIGHBOR_NW) | (1<<NEIGHBOR_NE) ;
   Neighbor.south = (1<<NEIGHBOR_S) | (1<<NEIGHBOR_SW) | (1<<NEIGHBOR_SE) ;
   Neighbor.east  = (1<<NEIGHBOR_E) | (1<<NEIGHBOR_SE) | (1<<NEIGHBOR_NE) ;
   Neighbor.west  = (1<<NEIGHBOR_W) | (1<<NEIGHBOR_SW) | (1<<NEIGHBOR_NW) ;

}
