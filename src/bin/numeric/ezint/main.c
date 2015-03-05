
#include	"ipw.h"
#include 	"pgm.h"

#define	NMAX	40960

int
main(
	int	argc,
	char	**argv)
{
	static OPTION_T opt_t = {
		't', "start time for integration or averaging",
		REAL_OPTARGS, "start",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_i = {
		'i', "integration or averaging interval",
		REAL_OPTARGS, "interval",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_n = {
		'n', "estimate of number of input pairs (default: 40960)",
		INT_OPTARGS, "count",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_a = {
		'a', "output averages (default: output integrated values)",
	};

	static OPTION_T opt_h = {
		'h', "units for time values is decimal hours (default: days)",
	};

	static OPTION_T opt_s = {
		's', "units for time values is decimal seconds (default: days)",
	};

	static OPTION_T *optv[] = {
		&opt_t,
		&opt_i,
		&opt_n,
		&opt_a,
		&opt_h,
		&opt_s,
		0
	};

	int	ave;
	int	hr;
	int	i;
	int	nin;
	int	nmax;
	int	nout;
	int	sec;

	double	start;
	double	tt;
	double	intlen;
	double	intend;
	double	t_thresh;
	double	yhold;
	double	nextyval;
	double	yval;
	double	dt;
	double	temp;

	double	*tin;
	double	*yin;


	/*	get args	*/

	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

	start = real_arg(opt_t, 0);
	intlen = real_arg(opt_i, 0);

        if (got_opt(opt_n)) {
                nmax = int_arg(opt_n, 0);
        }
        else {
		nmax = NMAX;
        }

	ave = got_opt(opt_a);
	hr  = got_opt(opt_h);
	sec = got_opt(opt_s);

	/*	 set storage for tin, and yin	*/

	if ((tin=(double *)(ecalloc(nmax, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of time values");
	 }

	if ((yin=(double *)(ecalloc(nmax, sizeof(double)))) == NULL) {
		error("cannot allocate memory for array of y values");
	 }

	/*	read input time-y pairs	*/

	for (nin = 0; nin < NMAX; nin++) {
		if (scanf("%lf %lf", &tin[nin], &yin[nin]) == EOF)
			break;
	}
	if (nin >= NMAX) {
		if (scanf("%lf", &temp) != EOF)
			error("too many input pairs (more than %d)", nmax);
	}

	/*	release unused space in tin and yin	*/

	if ((tin=(double *)(realloc(tin,nin*sizeof(double))))==NULL) {
		error("cannot release unused space in array of time values");
	}

	if ((yin=(double *)(realloc(yin,nin*sizeof(double))))==NULL) {
		error("cannot release unused space in array of y values");
	}

	/*	set t_thresh	*/

	t_thresh = ((tin[nin-1] - tin[0]) / nin) * 1.25;

	/*	check start	*/

	if (start < (tin[0] - t_thresh))
		error("start time early, start=%.5f", start);
	
	if ((start + intlen) > tin[nin-1])
		error("start time late, start=%.5f", start);

	/*	put times into seconds for integration	*/
	if (hr) {
		start *= 3600.0;
		intlen *= 3600.0;
		for (i=0; i<nin; i++)
			tin[i] *= 3600.0;
	}
	else if (!sec) {
		start *= 86400.0;
		intlen *= 86400.0;
		for (i=0; i<nin; i++)
			tin[i] *= 86400.0;
	}

	/*	set nout	*/

	nout = (tin[nin-1] - start) / intlen;

	if (nout >= nin)
		error("integration interval <= data interval");

	/**	do integration (or averaging)	**/

	tt = start;
	intend = start + intlen;
	yval = yhold = nextyval = 0.0;
	i = 0;

	while (intend <= tin[nin-1]) {
		/*	search for start in input file	*/
		if (i==0 && tt<tin[i]) { /* start < tin[0] */

			/* compute first yval */
			dt = tin[i] - tt;
			yhold = yin[i] * dt;

			/* store first yval */
			yval = yhold;

			tt = tin[i];
			i++;
		}
		else if (i==0) { /* start => tin[0] */
			/* advance to start */
			while (tt >= tin[i])
				i++;

			/* compute first yval */
			dt = tin[i] - tt;
			/* check time offset */
			if (dt > t_thresh) { /* to large, adjust */
				tt = tin[i] - t_thresh;
				/* forward segment */
				dt = t_thresh;
				yhold = yin[i] * dt;
				/* back segment */
				dt = tt - tin[i-1];
				yhold += yin[i-1] * dt;
			}
			else /* ok, no adjustment */
				yhold = yin[i] *dt;

			/* store first yval */
			yval = yhold;

			tt = tin[i];
			i++;
		}

		/* compute central part of the integral */
		while (tin[i] < intend) {
			dt = tin[i] - tin[i-1];
			/* check time offset */
			if (dt > t_thresh) { /* to large, adjust */
				tt = tin[i] - t_thresh;
				/* forward segment */
				dt = t_thresh;
				yhold = yin[i] * dt;
				/* back segment */
				dt = tt - tin[i-1];
				yhold += yin[i-1] * dt;
			}
			else /* ok, no adjustment */
				yhold = yin[i] * dt;

			/* store yval */
			yval += yhold;

			tt = tin[i];
			i++;
		}

		/* compute last yval and nextyval */
		tt = intend;
		dt = tin[i] - tin[i-1];
		/* check time offset */
		if (dt > t_thresh) { /* to large, adjust */
			/* compute interval value */
			nextyval = yin[i] * t_thresh;
			nextyval += yin[i-1] * (dt - t_thresh);

			/*  offset >= (t_thresh + intend) */
			if ((tin[i] - t_thresh) >= intend) {
				/* compute yval */
				dt = intend - tin[i-1];
				yhold = yin[i-1] * dt;
			}
			/* offset < (t_thresh + intend) */
			else if ((tin[i] - t_thresh) < intend) {
				/* forward segment of yval */
				dt = intend - (tin[i] - t_thresh);
				yhold = yin[i] * dt;
				/* back segment of yval */
				dt = (tin[i] - t_thresh) - tin[i-1];
				yhold += yin[i-1] * dt;
			}

		}
		else { /* ok, no adjustment */
			/* compute interval value */
			nextyval = yin[i] * dt;
			/* compute yval */
			dt = intend - tin[i-1];
			yhold = yin[i] * dt;
		}

		/* store and save yval */
		yval += yhold;

		if (ave)
			yval /= intlen;

		/* write integration start time and integral */
		if (sec)
			printf ("%.1f\t%.4f\n", start, yval);
		else if (hr)
			printf ("%.3f\t%.4f\n", start/3600.0, yval);
		else
			printf ("%.5f\t%.4f\n", start/86400.0, yval);

		/* compute first segment of next integral */
		yval = nextyval - yhold;

		/* re-set limits of integration */
		start = intend;
		intend += intlen;
		tt = tin[i];
		i++;
	}
	ipwexit (EXIT_SUCCESS);
}
