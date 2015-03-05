/*
** NAME
**      setCmdBuf, putCmd, putCmdAndInt, putCmdAndFloat, setDest, numCmds --
**		routines to store commands in a command buffer
**
** SYNOPSIS
**	#include "pgm.h"
**
**	void
**	setCmdBuf(
**		command_t *buffer,
**		int	   length)
**
**	command_t *
**	putCmd(
**		int code)
**
**	command_t *
**	putCmdAndInt(
**		int code,
**		int value)
**
**	command_t *
**	putCmdAndFloat(
**		int   code,
**		float value)
**
**	void
**	setDest(
**		command_t * cmdLoc)
**
**	int
**	numCmds(void)
**
**	bool_t
**	oneNumCmd(void)
**
** DESCRIPTION
**	These routines store commands in a command buffer.  The
**	routine "setCmdBuf" specifies the buffer and its size.
**	The "put"* routines are used to store commands.
**
**	"putCmd"         stores just a command (no arguments).
**	"putCmdAndInt"   stores a command with one integer argument.
**	"putCmdAndFloat" stores a command with one float argument.
**
**	The "setDest" sets the integer argument of the specified
**	command to be the offset between the next open spot in the
**	buffer and the given command.  Used with GOTO commands.
**
** RETURN VALUE
**	The "put"* routines return a pointer to the command that was
**	just stored.  This pointer is usually only saved with GOTO
**	commands; in such cases, the pointer is then passed to as an
**	argument to "setDest".
**
**	The "numCmds" routines returns the number of commands currently
**	in the buffer.
**
**	The "oneNumCmd" routine returns TRUE if the command buffer
**	contains just one number command.  Used to determine if an
**	expression has been reduced to a single constant.
**
** GLOBAL VARIABLES READ 
**
** GLOBAL VARIABLES MODIFIED
*/

#include	"ipw.h"
#include        "pgm.h"
#include        "command.h"
#include        "cmdbuf.h"

/* ------------------------------------------------------------------------ */

static command_t *cmdBuf;	/* buffer to store commands	       */
static int	  bufLen;	/* length of given command buffer      */
static int	  cmdCount = 0;	/* # of commands in buffer	       */
static command_t *nextCmd;	/* position for next command in buffer */

/*
 *  private local function
 */
static void checkEvalNow(void);

/* ------------------------------------------------------------------------ */

void
setCmdBuf(
	command_t *buffer,
	int	   length)
{
	assert(buffer != NULL);
	assert(length > 0);

	cmdBuf = buffer;
	bufLen = length;

	cmdCount = 0;
	nextCmd = buffer;
}

/* ------------------------------------------------------------------------ */

command_t *
putCmd(
	int code)
{
	if (cmdCount == bufLen) {
		error("Buffer overflow");
	}

	nextCmd->code = code;
	cmdCount++;
	nextCmd++;

	/*
	 *  For certain command codes, check if its operands are numbers.
	 *  The one or two operands will be the last one or two commands
	 *  in the buffer.  If the operands are numbers, then actually
	 *  evaluate the command now, and replace the operands with the
	 *  result.
	 */
	checkEvalNow();

	return nextCmd - 1;
}

/* ------------------------------------------------------------------------ */

/*
 *  This private routine checks to see if the last command placed in the
 *  buffer can be evaluated now.  For certain commands, the command can
 *  be evaluated now if its operands (which are the preceeding commands
 *  in buffer) are numbers.
 */

static void
checkEvalNow(void)
{
	command_t * lastCmd;
	command_t * last2Cmds;
	command_t * last3Cmds;
	fpixel_t    result;
	int	    nArgs;
	command_t * cmd;

	/*
	 *  Command codes that canNOT be evaluated now.
	 */
	if (cmdCount < 1)
	    return;
	lastCmd = nextCmd - 1;
	switch (lastCmd->code) {
	    case CMD_BAND:
	    case CMD_BAND_AND_NUM:
	    case CMD_E:
	    case CMD_FLOAT:
	    case CMD_GOTO:
	    case CMD_GOTO_IF_0:
	    case CMD_INTEGER:
	    case CMD_NBANDS:
	    case CMD_PI:
	    case CMD_SUM:
		return;

	    case CMD_MAX:
	    case CMD_MEDIAN:
	    case CMD_MIN:
		/*
		 *  Evaluate these functions only if all their arguments
		 *  are numbers.
		 */
		nArgs = IntValue(lastCmd);
		if (nArgs > 0) {
		    for (cmd = lastCmd - nArgs; cmd != lastCmd; cmd++)
			switch (cmd->code) {
			    case CMD_E:
			    case CMD_FLOAT:
			    case CMD_INTEGER:
			    case CMD_PI:
				break;
			    default:
				/*
				 *  We do NOT have a number for an argument.
				 */
				return;
			}

		    /*
		     *  Invoke the interpreter on the last N commands in buffer
		     *	where N is # of arguments + 1 (for the command itself).
		     *  Replace these commands with the result.
		     */
		    lastCmd -= nArgs;
		    if (execute(lastCmd, nArgs + 1, (fpixel_t *) NULL, 0,
				&result)) {
			cmdCount -= nArgs;
			lastCmd->code = CMD_FLOAT;
			FloatValue(lastCmd) = result;
			nextCmd = lastCmd + 1;
		    } else {
			error("error while evaluating constants");
		    }
		}  /* if */
		return;
	}  /* switch */

	/*
	 *  Command codes that take a single operand.
	 */
	if (cmdCount < 2)
	    return;
	last2Cmds = lastCmd - 1;
	switch (last2Cmds->code) {
	    case CMD_E:
	    case CMD_FLOAT:
	    case CMD_INTEGER:
	    case CMD_PI:
		/*
		 *  We have a number for an operand.
		 */
		break;
	    default:
		return;
	}

	switch (lastCmd->code) {
	    case CMD_ARCCOS:
	    case CMD_ARCSIN:
	    case CMD_ARCTAN:
	    case CMD_COS:
	    case CMD_LOG:
	    case CMD_LOG10:
	    case CMD_NEGATE:
	    case CMD_NOT:
	    case CMD_ROUND:
	    case CMD_SIN:
	    case CMD_TAN:
	    case CMD_TRUNC:
		/*
		 *  Invoke the interpreter on the last 2 commands in buffer.
		 *  Replace these commands with the result.
		 */
		if (execute(last2Cmds, 2, (fpixel_t *) NULL, 0, &result)) {
		    cmdCount--;
		    last2Cmds->code = CMD_FLOAT;
		    FloatValue(last2Cmds) = result;
		    nextCmd--;
		} else {
		    error("error while evaluating constants");
		}
		return;
	}  /* switch */

	/*
	 *  Command codes that take two operands.
	 */
	if (cmdCount < 3)
	    return;
	last3Cmds = last2Cmds - 1;
	switch (last3Cmds->code) {
	    case CMD_E:
	    case CMD_FLOAT:
	    case CMD_INTEGER:
	    case CMD_PI:
		/*
		 *  We have a number for second operand.
		 *  Actually this earlier command is the left operand
		 *  for these binary commands.
		 */
		break;
	    default:
		return;
	}

	switch (lastCmd->code) {
	    case CMD_ADD:
	    case CMD_AND:
	    case CMD_DIVIDE:
	    case CMD_EQUAL:
	    case CMD_EXPONENTATION:
	    case CMD_GREATER:
	    case CMD_GREATER_EQUAL:
	    case CMD_LESS:
	    case CMD_LESS_EQUAL:
	    case CMD_MULTIPLY:
	    case CMD_NOT_EQUAL:
	    case CMD_OR:
	    case CMD_SUBTRACT:
		/*
		 *  Invoke the interpreter on the last 3 commands in buffer.
		 *  Replace these commands with the result.
		 */
		if (execute(last3Cmds, 3, (fpixel_t *) NULL, 0, &result)) {
		    cmdCount -= 2;
		    last3Cmds->code = CMD_FLOAT;
		    FloatValue(last3Cmds) = result;
		    nextCmd -= 2;
		} else {
		    error("error while evaluating constants");
		}
		return;
	}  /* switch */
}

/* ------------------------------------------------------------------------ */

command_t *
putCmdAndInt(
	int code,
	int value)
{
	if (cmdCount == bufLen) {
		error("Buffer overflow");
	}

	nextCmd->code = code;
	IntValue(nextCmd) = value;
	cmdCount++;
	nextCmd++;

	/*
	 *  For certain functions that accept a variable number of arguments,
	 *  see if we can evaluate the function now.
	 */
	switch (code) {
	    case CMD_MAX:
	    case CMD_MEDIAN:
	    case CMD_MIN:
		checkEvalNow();
	}

	return nextCmd - 1;
}

/* ------------------------------------------------------------------------ */

command_t *
putCmdAndFloat(
	int   code,
	float value)
{
	if (cmdCount == bufLen) {
		error("Buffer overflow");
	}

	nextCmd->code = code;
	FloatValue(nextCmd) = value;
	cmdCount++;
	return nextCmd++;
}

/* ------------------------------------------------------------------------ */

void
setDest(
	command_t * cmdLoc)
{
	assert(cmdLoc != NULL);

	IntValue(cmdLoc) = nextCmd - cmdLoc;
}

/* ------------------------------------------------------------------------ */

int
numCmds(void)
{
	return cmdCount;
}

/* ------------------------------------------------------------------------ */

bool_t
oneNumCmd(void)
{
	if (cmdCount != 1)
		return FALSE;

	switch (cmdBuf->code) {
	    case CMD_E:
	    case CMD_FLOAT:
	    case CMD_INTEGER:
	    case CMD_PI:
		return TRUE;
	    default:
		return FALSE;
	}
}
