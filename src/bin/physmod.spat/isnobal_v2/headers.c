/*
**
** NAME
** 	headers -- read input image headers
** 
** SYNOPSIS
** 
**	void
**	headers(void)
** 
** DESCRIPTION
** 	headers reads and checks the headers of the input images 
** 
** GLOBAL VARIABLES READ
**	fdic
**	fdm
**	ic_bih
**	ic_geoh
**	restart
**
** GLOBAL VARIABLES MODIFIED
**
*/

#include "ipw.h"
#include "bih.h"
#include "lqh.h"
#include "geoh.h"
#include "fpio.h"
#include "gethdrs.h"
#include "pgm.h"

/*
 *  expected units for initial conditions image
 */

UNITS_T init_units[] = {
	U_METERS,
	U_METERS,
	U_METERS,
	U_KILOGRAMS_PER_CUBIC_METER,
	U_CELSIUS,
	U_CELSIUS,
	U_PERCENT 
};

UNITS_T restart_units[] = {
	U_METERS,
	U_METERS,
	U_METERS,
	U_KILOGRAMS_PER_CUBIC_METER,
	U_CELSIUS,
	U_CELSIUS,
	U_PERCENT,
	U_CELSIUS
};


void
headers(void)
{
	BIH_T         **m_bih;		/* -> mask BIH			 */

	static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
	static GETHDR_T h_geoh = {GEOH_HNAME, (ingest_t) geohread};
	static GETHDR_T *request[] = {&h_lqh, &h_geoh, 0};


   /* read BIH of init/restart image */

	if ((ic_bih = bihread(fdic)) == NULL) {
		error("can't read initial conditions BIH");
	}

   /* check init/restart BIH */

	if (restart) {
		if (hnbands(fdic) != ICBANDS_RESTART) {
			error("initial conditions image for restarting must "
			      "have %d bands", ICBANDS_RESTART);
		}
	} else {
		if (hnbands(fdic) != ICBANDS) {
			error("initial conditions image must have %d bands",
			      ICBANDS);
		}
	}

   /* read BIH of mask image */

	if (fdm != ERROR) {
		if ((m_bih = bihread(fdm)) == NULL) {
			error("can't read mask BIH");
		}

		if (bih_nlines(ic_bih[0]) != bih_nlines(m_bih[0]) ||
		    bih_nsamps(ic_bih[0]) != bih_nsamps(m_bih[0]))
			error("mask and init/restart images are incompatible");

		if (bih_nbands(m_bih[0]) != 1)
			error("mask image has > 1 bands");

		skiphdrs(fdm);
	}

   /* ingest LQH and GEOH, other headers are skipped */

	gethdrs(fdic, request, NO_COPY, ERROR);

	if (hdr_addr(h_lqh) == NULL)
		warn("input file has no LQH; raw values used");
	else if (units_warn) {
		if (restart)
			check_units((LQH_T **) hdr_addr(h_lqh), restart_units,
				    ICBANDS_RESTART, fdic);
		else
			check_units((LQH_T **) hdr_addr(h_lqh), init_units,
				    ICBANDS, fdic);
	}

	ic_geoh = (GEOH_T **) hdr_addr(h_geoh);
}
