#ifndef COMPILE_H
#define COMPILE_H

#include "command.h"

#define PARSE_OK	0
#define PARSE_NOTHING	1
#define PARSE_ERROR	2

extern bool_t constantExpr;

extern int expression(void);
extern int compile(char *str, bool_t strIsExpr, command_t * buffer, int length);

#endif  /* COMPILE_H */
