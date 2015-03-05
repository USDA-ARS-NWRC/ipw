#ifndef PGM_H
#define PGM_H

#include "IPWmacros.h"
#include "lqh.h"

 /*
  * functions used
  */
extern void	headers(int fdg, int fdm, int fdo, double *blat,
			double *blon, double *dlat, double *dlon);
extern LQH_T  **newlqh(int fdo);
extern void     gsunlight(int fdm, int fdo, int year, int month, int day,
			  int hour, int min, int sec, double blat,
			  double blon, double dlat, double dlon);

 /*
  * constants
  */
#define IBANDS	2
#define OBANDS	2

#endif
