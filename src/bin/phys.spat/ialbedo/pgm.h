#ifndef PGM_H
#define PGM_H

/* Header file for program ialbedo */

#include "IPWmacros.h"

#define MAX_GSIZE	 500.0
#define MAX_MAXGSZ	2000.0
#define MIN_DIRT	   1.0
#define MAX_DIRT	  10.0

#define MAXV	1.0		/* vis albedo when gsize = 0		 */
#define MAXIR	0.85447		/* IR albedo when gsize = 0		 */
#define IRFAC	-0.02123 	/* IR decay factor			 */
#define VFAC	500.0		/* visible decay factor			 */
#define VZRG	1.375e-3	/* vis zenith increase range factor	 */
#define IRZRG	2.0e-3		/* ir zenith increase range factor	 */
#define IRZ0	0.1		/* ir zenith increase range, gsize=0	 */

extern void	newlqh(int fdo, double vmin, double vmax, double irmin,
		       double irmax, bool_t vis_only, bool_t ir_only);
extern void	headers(int fdi, int fdo, int fds, int obands);
extern void	ialbedo_map(int fdi, int fdo, int fds, double day,
			double gsize, double maxgsz, double dirt,
			bool_t vis_only, bool_t ir_only);
extern void	ialbedo(int fdi, int fdo, double start, double day,
			double gsize, double maxgsz, double dirt,
			bool_t vis_only, bool_t ir_only);
extern double	growth(double t);

#endif
