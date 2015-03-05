
#include	"ipw.h"
#include 	"pgm.h"

#define	NMAX	40960

int
main(
	int	argc,
	char	**argv)
{
        static OPTION_T opt_s = {
		's', "segment start and end values for integral weighting",
                REAL_OPTARGS, "#",
                REQUIRED, 2, 2
        };

        static OPTION_T opt_n = {
                'n', "estimate of max number of input pairs (default: 40960)",
                INT_OPTARGS, "count",
                OPTIONAL, 1, 1
        };

	static OPTION_T *optv[] = {
		&opt_s,
		&opt_n,
		0
	};

	int	i;
	int	j;
	int	nin;	/**	number of input pairs			**/
	int	nj;	/**	number of pairs in segment		**/
	int	nmax;	/**	max number of input pairs allowed	**/

	j = 0;		/**	initialize j counter			**/

	double	end;		/**	segment end		**/
	double	p;		/**	1st interval proportion **/
	double	start;		/**	segment start		**/	
	double	sum_flux;	/**	sum segment flux	**/
	double	temp;

	sum_flux = 0.0;		/**	zero sum_flux		**/

	double	*flux;		/**	segment flux		**/
	double	*flux_den;	/**	segment y-values	**/
	double	*wave;		/**	segment x-values	**/
	double	*weight;	/**	segment weights		**/
	double	*xin;		/**	input x-values		**/
	double	*yin;		/**	input y-values		**/


	/*	get args	*/

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	/*	set segment range and weighting interval	*/

	start = real_arg(opt_s, 0);
	end = real_arg(opt_s, 1);

        if (got_opt(opt_n)) {
                nmax = int_arg(opt_n, 0);
        }
        else {
                nmax = NMAX;
        }

	/*	 set storage for xin, and yin	*/

	if ((xin=(double *)(ecalloc(nmax, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of x values");
	 }

	if ((yin=(double *)(ecalloc(nmax, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of y values");
	 }

        /*      read input x-y pairs */

        for (nin = 0; nin < nmax; nin++) {
                if (scanf("%lf %lf", &xin[nin], &yin[nin]) == EOF)
                        break;
        }
        if (nin >= nmax) {
                if (scanf("%lf", &temp) != EOF)
                        error("too many input pairs (more than %d)", nmax);
        }

        /*      release unused space in xin and yin     */

        if ((xin=(double *)(realloc(xin,nin*sizeof(double))))==NULL) {
                error("cannot release unused space in array of time values");
        }

        if ((yin=(double *)(realloc(yin,nin*sizeof(double))))==NULL) {
                error("cannot release unused space in array of y values");
        }

	/*	 set storage for flux, flux_den, wave, and weight	*/

	if ((flux=(double *)(ecalloc(nin, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of flux values");
	 }

	if ((flux_den=(double *)(ecalloc(nin, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of flux_den values");
	 }

	if ((wave=(double *)(ecalloc(nin, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of wave values");
	 }

	if ((weight=(double *)(ecalloc(nin, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of weight values");
	 }



	/*	check weighting range against input data	*/

	if (start < xin[0]) {
		error("segment start (%.6f) < initial spectra value (%.6f)",
			start, xin[0]);
	}
	
	if (end > xin[nin-1]) {
		error("segment (%.6f) > final spectra value (%.6f)",
			end, xin[nin-1]);
	}

	/**	calculate weights	**/

	for (i = 0; i < nin; i++) {

		/**	in the segment range??	**/
		if ((xin[i] >= start) && (xin[i] <= end)) {

			/* first weight(s) */
			if (j == 0) {

				/* if start falls between data intervals */
				if (xin[i] > start) {

					p = (xin[i] - start) / (xin[i] - xin[i-1]);
					flux[j] =  p * ((xin[i] - xin[i-1]) * yin[i-1]);
					flux_den[j] = (yin[i-1] + yin[i]) / 2.0;
					wave[j] = start;
					sum_flux += flux[j];
					j = 1;
					flux[j] = ((xin[i+1] - xin[i]) * yin[i]);
					flux_den[j] = yin[i];
					wave[j] = xin[i];
					sum_flux += flux[j];
					j = 2;

				} else {

				/* if start falls on a data interval */
				flux[j] = ((xin[i+1] - xin[i]) * yin[i]);
				flux_den[j] = yin[i];
				wave[j] = xin[i];
				sum_flux += flux[j];
				j = 1;
				}
			} else {

				if (xin[i] == end) {

				/* end falls on a data interval */
					break;
				} else if ((xin[i] != end) && (xin[i+1] > end)) {

				/* if end falls between data intervals */
					flux[j] = (end - xin[i]) * yin[i];
				} else {

				/* everything in between */
					flux[j] = (xin[i+1] - xin[i]) * yin[i];
				}
				flux_den[j] = yin[i];
				wave[j] = xin[i];
				sum_flux += flux[j];
				j++;
			}
		}
	}
	
	/* set number of weights in segment */
	nj = j;

	for (i = 0; i < nj; i++) {
		weight[i] = flux[i] / sum_flux;
		printf ("%g\t%g\n",
			wave[i], weight[i]);
	}

	ipwexit (EXIT_SUCCESS);
}
