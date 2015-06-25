#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "fpio.h"
#include "lqh.h"

/*
 ** NAME
 **	init_tau -- read headers and initialize lookup tables
 **
 ** SYNOPSIS
 **	init_tau(fdi, fdo, elev, mu0, tau, omega, gfact, R0, S0, nbits)
 **	int fdi;
 **	int fdo;
 **	double elev;
 **	double mu0;
 **	double tau;
 **	double omega;
 **	double gfact;
 **	double R0;
 **	double S0;
 **	int *nbits;
 **
 */

#define OBANDS	2

void
init_tau(
		int             fdi,		/* input file descriptor	 */
		int             fdo,		/* output file descriptor	 */
		double          elev,		/* elevation of tau meas	 */
		double          mu0,		/* cosine solar zenith		 */
		double          tau,		/* reference optical depth	 */
		double          omega,		/* single-scattering albedo	 */
		double          gfact,		/* asymmetry parameter		 */
		double          R0,		/* mean surface albedo		 */
		double          S0,		/* exoatmospheric solar irrad	 */
		int            *nbits)		/* # output bits		 */
{
	BIH_T         **i_bih;		/* input BIH			 */
	BIH_T         **o_bih;		/* output BIH			 */
	LQH_T         **o_lqh;		/* output LQH			 */
	fpixel_t        bval[2];	/* min,max beam rad		 */
	fpixel_t        dval[2];	/* min,max diffuse rad		 */
	pixel_t         ival[2];	/* min,max pixel values		 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T *request[] = {&h_lqh, 0};

	/*
	 * ingest BIH
	 */
	if ((i_bih = bihread(fdi)) == NULL)
		error("reading BIH");
	if (nbits[0] == 0)
		nbits[0] = hnbits(fdi, 0);
	if (nbits[1] == 0)
		nbits[1] = nbits[0];
	if (hnbands(fdi) != 1)
		error("input image can have only 1 band");
	/*
	 * write BIH
	 */
	/* NOSTRICT */
	o_bih = (BIH_T **)
						hdralloc(OBANDS, sizeof(BIH_T *), fdo, BIH_HNAME);
	assert(o_bih != NULL);
	o_bih[0] = bihmake(0, nbits[0],
			bih_history(i_bih[0]), bih_annot(i_bih[0]),
			o_bih[0],
			bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			OBANDS);
	assert(o_bih[0] != NULL);
	o_bih[1] = bihmake(0, nbits[1],
			bih_history(i_bih[0]), bih_annot(i_bih[0]),
			o_bih[0],
			bih_nlines(i_bih[0]), bih_nsamps(i_bih[0]),
			OBANDS);
	if (bihwrite(fdo, o_bih) == ERROR) {
		error("can't write new BIH");
	}

	/*
	 * ingest LQH
	 */
	gethdrs(fdi, request, 1, fdo);
	/*
	 * radiation look-up tables, min & max beam and diffuse radiation are
	 * returned in bval, dval
	 */
	radtbls(fdi, elev, mu0, tau, omega, gfact, R0, S0,
			bval, dval);
	/*
	 * output LQH
	 */
	/* NOSTRICT */
	o_lqh = (LQH_T **) hdralloc(OBANDS, sizeof(LQH_T *), fdo, LQH_HNAME);

	ival[0] = 0;
	ival[1] = ipow2(nbits[0]) - 1;
	o_lqh[0] = lqhmake(nbits[0], 2, ival, bval, "W m^-2", (char *) NULL);

	ival[1] = ipow2(nbits[1]) - 1;
	o_lqh[1] = lqhmake(nbits[1], 2, ival, dval, "W m^-2", (char *) NULL);

	if (lqhwrite(fdo, o_lqh) == ERROR) {
		error("can't write LQH");
	}

	/*
	 * get ready for rest of output
	 */
	if (boimage(fdo) == ERROR) {
		error("can't terminate header output");
	}
}
