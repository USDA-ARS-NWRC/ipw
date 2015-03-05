#ifndef	_SOLAR_H
#define	_SOLAR_H

#define	SUNRISE		1
#define	SUNSET		2

/* ------------------------------------------------------------------------- */

/*
 *  These routines are internal to this directory.
 */

extern datetime_t *	_sunpos(double lat, double lon, int year, int month,
				int day, int flag);

#endif
