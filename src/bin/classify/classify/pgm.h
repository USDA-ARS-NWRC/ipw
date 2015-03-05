#include "IPWmacros.h"

#define MAXCLASS 255

#define NO_DATA 0

#include "crh.h"

void		get_classes(FILE *fp, int *nclass, CLASS *classes);
void            headers(int fdi, int fdh, int fdm, FILE *ifp, int gotcrh,
			CLASS *classes, int *nclass, int *floor, int *ceil,
			int fdo);
void            classify(int fdi, int fdm, int nclass, CLASS *classes,
			pixel_t floor, pixel_t ceil, int fdo);
void		lutximg(int i_fd, int m_fd, int nlines, int nsamps,
			REG_3 pixel_t *lut, int o_fd);
