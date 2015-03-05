#ifndef PGM_H
#define PGM_H

#include	"IPWmacros.h"

#define	JD		1
#define	WY		2
#define	GMT		3
#define	DATE		4
#define	JD1		5
#define	WY1		6
#define	YMDHMS		7
#define	FMTSTR		8

#define DEFAULT_FMTSTR	"%Y_%m_%d+%H:%M:%S"

extern bool_t	cvtime(int in_fmt_type, char *in_fmt, int out_fmt_type,
			char *out_fmt, int yr, bool_t local_in,
			bool_t local_out, int zone, bool_t hrs, bool_t secs,
			char *datetime);

#endif
