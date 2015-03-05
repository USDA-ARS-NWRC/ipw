#ifndef _CELLSTACK_H
#define _CELLSTACK_H

#include "ipw.h"

/* 
   The cell stack structure is used to push and pop grid cell coordinates
   on a stack.  The stack stores the cell's x and y coordinates (sample and
   line respectively) and an index into a 1-D representation of grid.

   The cell stack structure was designed to allow recursive algorithms to
   be reimplemented iteratively.  The advantage is that the iterative 
   algorithm uses virtual memory to store the cells, whereas a recursive
   algorithm on a large grid can blow away the finit stack space pretty 
   quickly.

   It would be possible to implement the cell stack as macros so that the 
   iterative algorithm would be more efficient.
*/

typedef struct _CELL_STACK_ITEM
   {
   /* 
      This structure stores info about a specific grid cell 
      */
   int x;     /* X-coordinate (sample)                           */
   int y;     /* Y-coordinate (line)                             */
   int index; /* Index into 1-D array representation of the grid */
   }
CELL_STACK_ITEM ;

typedef struct _CELL_STACK
   {
   /*
      This structure stores the complete state information of the stack.

      The stack is broken up into a finite number of fixed-size buffers,
      each being allocated dynamically as the stack grows.  
      */
   CELL_STACK_ITEM **buffers; /* Array of buffers                          */
   int nbuffers;	      /* Size of the array of buffers              */
   int cur_buffer ;	      /* Buffer with the top element of the stack. */
   int cur_element;	      /* Index of top element within cur_buffer    */
   int max_allocated_buffer;  /* Pointer to most recently allocated buffer */
   }
CELL_STACK ;

/* 
   This is the number of cells to be stored in each dynamically allocated 
   if the number is too big, we could end up allocating way too much memory
   for the stack.  If the number is too small, the system will become 
   inefficient by requring a lot of extra malloc()'s.  
*/
#define CELL_STACK_BUFFER_SIZE 100000


extern void init_stack   (CELL_STACK ** stack, int numcells);
extern int  push_stack   (CELL_STACK *stack, int x, int y, int index);
extern int  pop_stack    (CELL_STACK *stack, int *x, int *y, int *index);
extern int  is_empty     (CELL_STACK *stack);
extern int  dealloc_stack(CELL_STACK *stack);

#endif 
