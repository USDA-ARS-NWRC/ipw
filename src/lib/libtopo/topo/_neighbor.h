#ifndef _NEIGHBOR_H
#define _NEIGHBOR_H

/*
   This structure and macros in this file define a mapping from neighbor
   codes to a cell's 8-neighbors.  This mapping is useful when a 2D grid 
   is represented by a 1-D vector of data.  The system is set up to accept any
   numbering of neighbors in the macros below.  During a call to 
   neighbor_init (neighbor.c), the NEIGHBOR_INFO structure is initialized
   relative to that numbering scheme and the grid size.

   Note that the compass directions here are for programmer's reference only.
   "North" is the direction towards the 0th line and "West" is the direction
   towards the 0th sample.  These DO NOT necessarily correspond to
   compass directions on the DEM.  
*/

#define NEIGHBOR_W	0
#define NEIGHBOR_NW	1
#define NEIGHBOR_N	2
#define NEIGHBOR_NE	3
#define NEIGHBOR_E	4
#define NEIGHBOR_SE	5
#define NEIGHBOR_S	6
#define NEIGHBOR_SW	7

typedef struct _neighbor_info
   {
   int   offset[8]    ;	/* Relative location in grid array to each neighbor */
   int   offset_x[8]  ;	/* Offset along x axis of each neighbor             */
   int   offset_y[8]  ;	/* Offset along y axis of each neighbor             */
   int   north        ; /* Bit mask of all neighbors in the -y direction    */
   int   south        ; /* Bit mask of all neighbors in the +y direction    */
   int   east         ; /* Bit mask of all neighbors in the +x direction    */
   int   west         ; /* Bit mask of all neighbors in the -x direction    */
   } 
NEIGHBOR_INFO ;

extern NEIGHBOR_INFO Neighbor;

extern void neighbor_init(int x_count);

#endif
