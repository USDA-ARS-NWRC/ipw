#ifndef CMDBUF_H
#define CMDBUF_H
 
/*
 *  The include file for command-buffer module.
 */
 
/* ------------------------------------------------------------------------ */

/*
 *  Routines for the command buffer.
 */

extern void	    setCmdBuf(command_t *buffer, int length);
extern command_t *  putCmd(int command);
extern command_t *  putCmdAndInt(int command, int value);
extern command_t *  putCmdAndFloat(int command, float value);
extern void	    setDest(command_t *cmdLoc);
extern int	    numCmds(void);
extern bool_t       oneNumCmd(void);

/* ------------------------------------------------------------------------ */
 
#endif
