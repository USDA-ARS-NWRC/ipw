#include "IPWmacros.h"
#include <math.h>

typedef long    hist_t;

hist_t         *hist(int i_fd, int m_fd, int h_nsamps);
void            histx(REG_1 pixel_t *i_buf, REG_3 pixel_t *m_buf,
		      REG_6 int nsamps, REG_2 hist_t *histo);
void            headers (int fdi, int fdm, int nclass, int fdo);
void            classeq(int fdi, int fdm, int fdo, int nclass, bool_t constr,
			fpixel_t lo, fpixel_t hi, pixel_t floor, pixel_t ceil,
			char *infile, char *maskfile, char rep_method);
fpixel_t	median(hist_t *hist, fpixel_t *values, pixel_t start_pixel,
			pixel_t end_pixel);
fpixel_t	mean(hist_t *hist, fpixel_t *values, pixel_t start_pixel,
			pixel_t end_pixel);
void		newcrh(int fdi, int fdo, fpixel_t *lo, fpixel_t *hi,
			fpixel_t *rep, pixel_t floor, pixel_t ceil, int nclass,
			char *annot);
void		lutximg(int i_fd, int m_fd, int nlines, int nsamps,
			REG_3 pixel_t *lut, int o_fd);

#define NO_DATA		0	/* class for no data value */

#define MEDIAN		0	/* methods for determining class */
#define MEAN		1	/* representative values         */
#define MIDPOINT	2
