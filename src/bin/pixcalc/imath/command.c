/*
 *  Module for command interpreter.
 */

#include <math.h>
#include <errno.h>

#include "ipw.h"
#include "pgm.h"
#include "command.h"

/* ------------------------------------------------------------------------ */

/*
** NAME
**      execute -- execute a sequence of interpreter commands
**
** SYNOPSIS
**	#include "command.h"
**
**	bool_t
**	execute(
**		command_t * commands,	|* buffer with sequence of commands *|
**		int         numCmds,	|* # of commands in buffer	    *|
**		fpixel_t  * bands,	|* array of fp-pixel values	    *|
**		int	    nBands,	|* # of pixel values in bands	    *|
**		fpixel_t  * result)	|* -> to where to put result        *|
**
** DESCRIPTION
**	This routine executes a sequence of interpreter commands on
**	a given sample of floating-point pixel values.
**
** RETURN VALUE
**	TRUE	Execution succeeded and result assigned.
**
**	FALSE	Error occurred.  Explanatory message stored via
**		"syserr" or "usrerr" routine.
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
*/

#ifdef CHECK_STACK

#define CHECK_OVERFLOW	if (top => endStack) { usrerr("Stack overflow"); \
						return FALSE; }
#define CHECK_1_VALUE	if (top < stack)     { usrerr("Stack underflow"); \
						return FALSE; }
#define CHECK_2_VALUES	if (top < stack + 1) { usrerr("Stack underflow"); \
						return FALSE; }
#define CHECK_N_VALUES(N)	\
			if (top < stack +((N)-1)) { usrerr("Stack underflow"); \
						return FALSE; }

#else

#define CHECK_OVERFLOW
#define CHECK_1_VALUE
#define CHECK_2_VALUES
#define CHECK_N_VALUES(N)

#endif

#define CHECK_ERRNO	if (errno) { syserr(); return FALSE; }
#define STACK_SIZE	50

/* ------------------------------------------------------------------------ */

/*
 *  A local routine to compare two doubles.  Used for sorting values
 *  to determine a median.
 */

static int
cmpDoubles(
	double * a,
	double * b)
{
	if (*a < *b)
	    return -1;
	else if (*a > *b)
	    return 1;
	else
	    return 0;
}

/* ------------------------------------------------------------------------ */

bool_t
execute(
	command_t * commands,	/* buffer with sequence of commands */
	int         numCmds,	/* # of commands in buffer	    */
	fpixel_t  * bands,	/* array of fp-pixel values	    */
	int	    nBands,	/* # of pixel values in bands	    */
	fpixel_t  * result)	/* -> to where to put result        */
{
	int	    index;	/* index of current command	    */
	command_t * cmd;	/* -> current command		    */
	int	    i;		/* band index			    */
	int	    nArgs;	/* # of arguments for a command     */
	double      sum;	/* sum of the bands (pixels)        */
	bool_t	    calcSum;	/* calculated sum already?	    */
	double      min;	/* minimum of the bands (pixels)    */
	bool_t	    calcMin;	/* calculated min already?	    */
	double      max;	/* maximum of the bands (pixels)    */
	bool_t	    calcMax;	/* calculated max already?	    */
	double    * sortedBands;/* sorted band values 		    */
	double      median;	/* median of the bands (pixels)     */
	bool_t	    calcMedian;	/* calculated median already?	    */


	static double  stack[STACK_SIZE];	/* expression stack */
	       double *top;			/* top of stack	    */
	       double *endStack;	   	/* end of stack	    */
	       double *p;		   	/* working stack ptr*/


	assert(commands != NULL);
	if (nBands > 0)
	    {
	    assert(bands != NULL);
	    }
	assert(result != NULL);

	calcSum = FALSE;
	calcMin = FALSE;
	calcMax = FALSE;
	calcMedian = FALSE;
	errno = 0;

	/*
	 *  Initialize stack
	 */
	top = stack - 1;
	endStack = stack + (STACK_SIZE - 1);

	/*
	 *  Start execution at first command in buffer.
	 */
	index = 0;

	while (index < numCmds)
	    {
	    cmd = commands + index;
	    switch (cmd->code)
		{
	     /*
	      *  Number commands
	      */
		case CMD_INTEGER:
		    CHECK_OVERFLOW
		    top++;
		    *top = IntValue(cmd);
		    break;

		case CMD_FLOAT:
		    CHECK_OVERFLOW
		    top++;
		    *top = FloatValue(cmd);
		    break;

	     /*
	      *  Constant commands
	      */
		case CMD_PI:
		    CHECK_OVERFLOW
		    top++;
		    *top = M_PI;
		    break;

		case CMD_E:
		    CHECK_OVERFLOW
		    top++;
		    *top = M_E;
		    break;

	     /*
	      *  Commands with 1 operand
	      *
	      *  Before		After
	      *
	      *	 top -> x	top -> fn(x)
	      *	        ...	       ...
	      */
		case CMD_NEGATE:
		    CHECK_1_VALUE
		    *top = -(*top);
		    break;

		case CMD_TRUNC:
		    CHECK_1_VALUE
		    *top = (int) (*top);
		    break;

		case CMD_ROUND:
		    CHECK_1_VALUE
		    *top = (int) ( *top + ((*top > 0) ? 0.5 : -0.5) );
		    break;

		case CMD_LOG:  /* log(x) (base e) */
		    CHECK_1_VALUE
		    *top = log(*top);
		    CHECK_ERRNO
		    break;

		case CMD_LOG10:  /* log(x) (base 10) */
		    CHECK_1_VALUE
		    *top = log10(*top);
		    CHECK_ERRNO
		    break;

	     /*
	      *  Trig functions
	      */
		case CMD_COS:
		    CHECK_1_VALUE
		    *top = cos(*top);
		    CHECK_ERRNO
		    break;

		case CMD_SIN:
		    CHECK_1_VALUE
		    *top = sin(*top);
		    CHECK_ERRNO
		    break;

		case CMD_TAN:
		    CHECK_1_VALUE
		    *top = tan(*top);
		    CHECK_ERRNO
		    break;

		case CMD_ARCCOS:
		    CHECK_1_VALUE
		    *top = acos(*top);
		    CHECK_ERRNO
		    break;

		case CMD_ARCSIN:
		    CHECK_1_VALUE
		    *top = asin(*top);
		    CHECK_ERRNO
		    break;

		case CMD_ARCTAN:
		    CHECK_1_VALUE
		    *top = atan(*top);
		    CHECK_ERRNO
		    break;

	     /*
	      *  Commands with 2 operands
	      *  (those implemented with augmented assignment operators)
	      *
	      *  Before		After
	      *
	      *	 top -> y
	      *	        x	top -> x operator y
	      *	        ...	       ...
	      */
		case CMD_ADD:
		    CHECK_2_VALUES
		    top--;
		    *top += *(top + 1);
		    break;

		case CMD_SUBTRACT:
		    CHECK_2_VALUES
		    top--;
		    *top -= *(top + 1);
		    break;

		case CMD_MULTIPLY:
		    CHECK_2_VALUES
		    top--;
		    *top *= *(top + 1);
		    break;

		case CMD_DIVIDE:
		    CHECK_2_VALUES
		    if (*top == 0.0)
			{
			usrerr("Division by zero");
			return FALSE;
			}
		    top--;
		    *top /= *(top + 1);
		    break;


	     /*
	      *  Commands with 2 operands
	      */
		case CMD_EXPONENTATION:
		    /*
		     *  Before		After
		     *
		     *	top -> y
		     *	       x	top -> x ^ y
		     *	       ...	       ...
		     */
		    CHECK_2_VALUES
		    top--;
		    *top = pow(*top, *(top + 1));
		    CHECK_ERRNO
		    break;

	     /*
	      *  Relational operator commands with 2 operands
	      *
	      *  Before		After
	      *
	      *	 top -> y
	      *	        x	top -> x relational_operator y
	      *	        ...	       ...
	      */
		case CMD_EQUAL:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top == *(top + 1)) ? 1 : 0;
		    break;

		case CMD_NOT_EQUAL:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top != *(top + 1)) ? 1 : 0;
		    break;

		case CMD_LESS:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top < *(top + 1)) ? 1 : 0;
		    break;

		case CMD_LESS_EQUAL:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top <= *(top + 1)) ? 1 : 0;
		    break;

		case CMD_GREATER:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top > *(top + 1)) ? 1 : 0;
		    break;

		case CMD_GREATER_EQUAL:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top >= *(top + 1)) ? 1 : 0;
		    break;

	     /*
	      *  Logical commands
	      */
		case CMD_NOT:
		    CHECK_1_VALUE
		    *top = (*top == 0) ? 1 : 0;
		    break;

		case CMD_AND:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top != 0 && *(top + 1) != 0) ? 1 : 0;
		    break;

		case CMD_OR:
		    CHECK_2_VALUES
		    top--;
		    *top = (*top != 0 || *(top + 1) != 0) ? 1 : 0;
		    break;

	     /*
	      *  Commands involving bands (pixels)
	      */
		case CMD_BAND:
		    /*
		     *  Before		After
		     *
		     *	top -> x	top -> bands[x]
		     *	       ...	       ...
		     */
		case CMD_BAND_AND_NUM:
		    /*
		     *  Before		After
		     *
		     *			top -> bands[index stored with cmd]
		     *	top -> ...	       ...
		     */
		    if (cmd->code == CMD_BAND)
			{
			CHECK_1_VALUE
			i = (int) (*top);
			}
		    else
			{
			CHECK_OVERFLOW
			top++;
			i = IntValue(cmd);
			}
		    if (i >= nBands)
			{
			usrerr("Accessing band %d, but only %d band%s",
			 	i, nBands, (nBands == 1) ? "" : "s");
			return FALSE;
			}
		    else if (i < 0)
			{
			usrerr("Negative band index: %d", i);
			return FALSE;
			}
		    *top = bands[i];
		    break;

		case CMD_SUM:
		    CHECK_OVERFLOW
		    if (! calcSum)
			{
			calcSum = TRUE;
			if (nBands <= 0)
			    {
			    sum = 0;
			    }
			else
			    {
			    for (i = 1, sum = bands[0]; i < nBands; i++)
				sum += bands[i];
			    }
			}
		    top++;
		    *top = sum;
		    break;

		case CMD_NBANDS:
		    CHECK_OVERFLOW
		    top++;
		    *top = nBands;
		    break;

	     /*
	      *  Commands with 0 or more operands
	      *
	      *  Before		After
	      *
	      *	 top -> arg n
	      *	        ...
	      *	        arg 2
	      *	        arg 1	top -> fn(arg 1, arg 2, ..., arg n)
	      *	        ...	       ...
	      */
		case CMD_MIN:
		    nArgs = IntValue(cmd);
		    if (nArgs > 0)
			{
		        CHECK_N_VALUES(nArgs)
			for (p = top - nArgs + 1; top > p; top--)
			    if (*top < *p)
				*p = *top;
			}
		    else
			{
			/*
			 *  No arguments (i.e., special variable), so
			 *  find the minimum value of the bands.
			 */
			CHECK_OVERFLOW
			if (! calcMin)
			    {
			    calcMin = TRUE;
			    if (nBands <= 0)
				{
				min = 0;
				}
			    else
				{
				for (i = 1, min = bands[0]; i < nBands; i++)
				    if (bands[i] < min)
					min = bands[i];
				}
			    }
			top++;
			*top = min;
			}
		    break;

		case CMD_MAX:
		    nArgs = IntValue(cmd);
		    if (nArgs > 0)
			{
		        CHECK_N_VALUES(nArgs)
			for (p = top - nArgs + 1; top > p; top--)
			    if (*top > *p)
				*p = *top;
			}
		    else
			{
			/*
			 *  No arguments (i.e., special variable), so
			 *  find the maximum value of the bands.
			 */
			CHECK_OVERFLOW
			if (! calcMax)
			    {
			    calcMax = TRUE;
			    if (nBands <= 0)
				{
				max = 0;
				}
			    else
				{
				for (i = 1, max = bands[0]; i < nBands; i++)
				    if (bands[i] > max)
					max = bands[i];
				}
			    }
			top++;
			*top = max;
			}
		    break;

		case CMD_MEDIAN:
		    nArgs = IntValue(cmd);
		    if (nArgs > 0)
			{
		        CHECK_N_VALUES(nArgs)
			SORT_ALG((void *) (top - nArgs + 1), (size_t) nArgs,
				 (size_t) sizeof(double), cmpDoubles);
			top -= nArgs - 1;
			p = top + nArgs/2;
			if (nArgs & 1)  	/* odd # of values */
			    *top = *p;
			else
			    *top = (*p + *(p - 1)) / 2.0;
			}
		    else
			{
			/*
			 *  No arguments (i.e., special variable), so
			 *  find the median value of the bands.
			 */
			CHECK_OVERFLOW
			if (! calcMedian)
			    {
			    calcMedian = TRUE;
			    if (nBands <= 0)
				{
				median = 0;
				}
			    else
				{
				if (sortedBands == NULL)
				    {
				    sortedBands = (double *) ecalloc(nBands,
								sizeof(double));
				    if (sortedBands == NULL)
					{
					usrerr("No memory to sort bands");
					return FALSE;
					}
				    }
				for (i = 0; i < nBands; i++)
				    sortedBands[i] = bands[i];
				SORT_ALG((void *) sortedBands, (size_t) nBands,
					 (size_t) sizeof(double), cmpDoubles);
				if (nBands & 1)  	/* odd # of bands */
				    median = sortedBands[nBands/2];
				else
				    median = (sortedBands[nBands/2] + 
					      sortedBands[nBands/2 - 1]) / 2.0;
				}
			    }
			top++;
			*top = median;
			}
		    break;

	     /*
	      *  Goto commands
	      */
		case CMD_GOTO:
		    index += IntValue(cmd);
		    continue;			/* jump to top of "while" */

		case CMD_GOTO_IF_0:
		    CHECK_1_VALUE
		    if (*top-- == 0)		/* pop value after testing */
			{
			index += IntValue(cmd);
			continue;		/* jump to top of "while" */
			}
		    break;

		default :
		    error("Unknown command code: %d", cmd->code);
		}  /* switch */

	    index++;
	    }  /* while */

	/*
	 *  Copy top of stack into result.
	 */
	if (top < stack)
	    {
	    usrerr("Stack underflow");
	    return FALSE;
	    }
	*result = (fpixel_t) (*top);

	return TRUE;
}

/* ------------------------------------------------------------------------ */

/*
** NAME
**      printCmd -- print an interpeter command
**
** SYNOPSIS
**	#include "command.h"
**
**	void
**	printCmd(
**		command_t *command)
**
** DESCRIPTION
**	printCmd prints the given command's name to standard input
**	along with any value.
**
** RETURN VALUE
**	None.
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
*/

#ifdef DEBUG

/*
 *  Command codes as text.
 */

static
char * commands[] = {
#include "cmdcodes.c"
};

/* ------------------------------------------------------------------------ */

void
printCmd(
	command_t *cmd)
{
	assert(cmd != NULL);

	printf("%s", commands[cmd->code]);
	switch (cmd->code) {
	    case CMD_INTEGER :
	    case CMD_BAND_AND_NUM :
	    case CMD_GOTO :
	    case CMD_GOTO_IF_0 :
		printf(" (%d)", IntValue(cmd));
		break;
	    case CMD_FLOAT :
		printf(" (%f)", FloatValue(cmd));
		break;
	    case CMD_MAX :
	    case CMD_MEDIAN :
	    case CMD_MIN :
		printf(" (# of args = %d)", IntValue(cmd));
		break;
	}  /* switch */
}

#endif
