#ifndef	PGM_H
#define	PGM_H

/*
 * header file for "imath" program
 */

#include "IPWmacros.h"

#include "image.h"
#include "command.h"

#define CMD_BUF_LEN 1000

/* ------------------------------------------------------------------------ */

/*
 *  Global variables
 */

extern	IMAGE_T *  i_image;		/* input image			 */
extern	IMAGE_T *  mask;		/* mask image			 */
extern	IMAGE_T *  o_image;		/* output image			 */

extern	int	   num_lines;		/* # of lines in image		 */
extern	int	   num_samples;		/* # of samples per line	 */
extern	int	   num_bands;		/* # of bands per sample	 */

extern	command_t  cmd_buf[CMD_BUF_LEN];
extern  int	   num_cmds;		/* # of commands in cmd_buf	 */

/* ------------------------------------------------------------------------ */

extern	char *	ordinal(int i);
extern	void	headers(float min, float max, char * units);
extern	void	imath(void);

#endif
