#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"

/*
 * header file for "imgstat" program
 */

typedef struct {
	fpixel_t	min;		/* min of unmasked pixels	 */
	fpixel_t	max;		/* max of unmasked pixels	 */
	fpixel_t	median;		/* median of unmasked pixels	 */
	double		mean;		/* mean of unmasked pixels	 */
	double		sdev;		/* stdev of unmasked pixels	 */
	long		npixels;	/* # of unmasked pixels		 */
	char *		units;		/* units			 */
} STAT;

typedef struct {
	fpixel_t	*min;		/* min of each class		 */
	fpixel_t	*max;		/* max of each class		 */
	double	        *sigma;		/* sum of pixels in each class	 */
	double	        *sigma2;	/* sum of square of pixels 	 */
	fpixel_t	*median;	/* median of each class 	 */
	double		*mean;		/* mean of each class		 */
	double		*sdev;		/* stdev of each class		 */
	long		*npixels;	/* # of pixels per class	 */
} CLS_STAT;


extern void     headers(int i_fd, int m_fd, bool_t class_stats,
			int *nclass, CLASS **classes, char **units);
extern void	statf(int fdi, int fdm, STAT *stat);
extern void	class_statf(int fdi, int fdm, STAT *stat, CLS_STAT *cls_stat,
			    int nclass, bool_t hist, FILE *hfp);
extern void     output(STAT stat, bool_t abbrev, bool_t print_units,
		       bool_t class_stats, CLS_STAT cls_stat, int nclass);

#endif
