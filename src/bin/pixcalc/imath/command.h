#ifndef COMMANDS_H
#define COMMANDS_H

/*
 *  Include file for command interpreter.
 */

/* ------------------------------------------------------------------------ */

/*
 *  Codes for the interpreter's commands.
 */

#include "cmdcodes.h"

/*
 *  Type definition for an interpreter command.
 */
typedef struct {
	int	code;	/* integer code for the command		*/
	union {
	   int i;
	   float f;
	}	value;	/* value for command: integer or float	*/
} command_t;

/*
 *  Macros for accessing a command's value
 */
#define	IntValue(cmdPtr)	( (cmdPtr)->value.i )
#define	FloatValue(cmdPtr)	( (cmdPtr)->value.f )

/* ------------------------------------------------------------------------ */

/*
 *  Routines for command interpreter
 */

extern void   printCmd(command_t * command);
extern bool_t execute(command_t * buffer, int length, fpixel_t * bands,
			int nBands, fpixel_t * result);

#endif  /* COMMANDS_H */
