#include <math.h>

#include "ipw.h"
#include "_cellstack.h"

void
init_stack(
      CELL_STACK ** stack,
      int	    numcells)
{
   /*
      The purpose of this procedure is to allocate a new stack structure with
      enough space in the stack buffer table for "numcells" entries.
      */

   /*
      Step 1.  Allocate stack structure and buffer arrays

      We need to make the buffer array large enough to hold "numcells"
      cells in the worst case.  In practice, the stack buffers will be
      allocated in CELL_STACK_BUFFER_SIZE chunks as needed.
      */
   *stack = (CELL_STACK *) ecalloc( 1, sizeof( CELL_STACK ) );
   if ( *stack == NULL )
      {
      error( "Cell stack allocation error\n" );
      }
   
   (*stack)->nbuffers = 
      (int) ( ( numcells * 8 ) / CELL_STACK_BUFFER_SIZE ) + 1 ;
   (*stack)->buffers = 
      (CELL_STACK_ITEM **) 
	 ecalloc( (*stack)->nbuffers, sizeof( CELL_STACK_ITEM *) ) ;

   if ( (*stack)->buffers == NULL)
      {
      error( "Cell stack buffer array allocation error\n" );
      }

   /*
      Step 2.  Initialize cur_buffer, cur_element 

      These are set to values such that on the first push_stack, the 
      first stack buffer will be allocated.
      */
   (*stack)->cur_buffer           = -1                         ;
   (*stack)->cur_element          = CELL_STACK_BUFFER_SIZE - 1 ; 
   (*stack)->max_allocated_buffer = -1                         ;
}

/* ------------------------------------------------------------------------ */

int
push_stack(
      CELL_STACK * stack,
      int          x,
      int          y,
      int          index)
{
   /*
      This procedure place a cell's coordinates at the top of the stack.  If
      the stack overflows its current buffer, a new buffer is allocated up
      until something larger than the worst-case maximum capacity indicated in
      the call to init_stack.  The return value is intended as a status 
      indicator; right now it always returns 0.
      */

   CELL_STACK_ITEM *cell ; /* Pointer to the (new) top element of the stack */

   /*fprintf(stderr,"Pushing %d %d %d\n", x, y, index );*/
   stack->cur_element++ ;
   if ( stack->cur_element >= CELL_STACK_BUFFER_SIZE )
      {
      /* Need to move to the next buffer */
      stack->cur_element = 0 ;
      stack->cur_buffer++    ;
      /*fprintf(stderr,"Need to move to buffer %d\n", stack->cur_buffer );*/

      if ( stack->cur_buffer > stack->max_allocated_buffer )
	 {
	 /*fprintf(stderr,"Need to allocate buffer %d\n",stack->cur_buffer );*/
	 /* Need to allocate a new buffer */
	 if ( stack->cur_buffer >= stack->nbuffers )
	    {
	    /*
	       Uh oh.  For some reason we're requiring a bigger stack than
	       we had planned for.  Maximum stack size is set in init_stack().
	       */
	    error( "Cell stack overflow error\n") ;
	    }
	 else
	    {
	    /*
	       Allocate the new buffer 
	       */
	    stack->buffers[ stack->cur_buffer ] = 
	       (CELL_STACK_ITEM *) 
		  ecalloc( CELL_STACK_BUFFER_SIZE, sizeof( CELL_STACK_ITEM) ) ;
	    if ( stack->buffers[ stack->cur_buffer ] == NULL )
	       {
	       error( "Cell stack buffer allocation error.\n" );
	       }
	    stack->max_allocated_buffer++;
	    }
	 }
      }
   /* 
      At this point, cur_buffer and cur_element point to the new top of 
      the stack.  This is a properly-allocated entry in the stack buffer 
      */
   cell   = & stack->buffers[ stack->cur_buffer ][ stack->cur_element ] ;
   cell->x =     x     ;
   cell->y =     y     ;
   cell->index = index ;
   return( 0 );
}

/* ------------------------------------------------------------------------ */

int
pop_stack(
      CELL_STACK * stack,
      int        * x,
      int        * y,
      int        * index)
{
   /*
      This procedure removes the top element of the stack and returns the
      cell coordinates in the x, y, and index values.  A 0 is returned if
      no error occurs; a -1 indicates and empty stack error.
      */

   if ( stack->cur_buffer == -1 )
      {
      /* Stack is empty */
      return( -1 );
      }
   else
      {
      CELL_STACK_ITEM *cell;  /* The cell at the top of the stack */

      /* 
	 Check that current buffer and element are legal.  NOTHING should
	 cause these assertions to fail
	 */
      assert( stack->cur_buffer <= stack->max_allocated_buffer && 
	      stack->cur_element >= 0 &&
	      stack->cur_element < CELL_STACK_BUFFER_SIZE ) ;

      /*
	 Pull the top element off the stack
	 */
      cell   = & stack->buffers[ stack->cur_buffer ][ stack->cur_element ] ;
      *x     = cell->x ;
      *y     = cell->y ;
      *index = cell->index ;

      /*fprintf(stderr,"Popping %d %d %d\n", *x, *y, *index );*/
      /*
	 Adjust pointers to the top element 
	 */
      stack->cur_element-- ;
      if ( stack->cur_element < 0 )
	 {
	 stack->cur_buffer-- ;
	 stack->cur_element = CELL_STACK_BUFFER_SIZE - 1 ;

	 /* 
	    Note: we could implement some genius deallocation scheme here,
	    but for now just make buffer allocation "permanent".
	    */
	 }

      return( 0 );
      }
}

/* ------------------------------------------------------------------------ */

int
is_empty(
      CELL_STACK * stack)
{
   /* 
      This procedure returns 1 if the given stack is empty; 0 otherwise
      */

   /* Use the cur_buffer pointer as an indicator of emptiness */
   if ( stack->cur_buffer == -1 )
      return( 1 );
   else
      return( 0 );
}

/* ------------------------------------------------------------------------ */

int
dealloc_stack(
      CELL_STACK * stack)
{
   /*
      This procedure de-allocates the memory associated with a particular 
      stack.  The return value is intended as a status 
      indicator; right now it always returns 0.
      */
   int i;

   /*
      Step 1: Deallocate stack buffers 
      */
   for ( i=0; i <= stack->max_allocated_buffer; i++)
      {
      free( stack->buffers[i] );
      }
   free( stack->buffers );

   /*
      Step 2: Deallocate stack structure
      */
   free( stack );

   return( 0 );
}
