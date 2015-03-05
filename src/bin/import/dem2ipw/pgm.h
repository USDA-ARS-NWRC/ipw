#ifndef	PGM_H
#define	PGM_H

#include "IPWmacros.h"
#include "ipw.h"
#include "fpio.h"
#include "pixio.h"

#define FEET_PER_METER	.3048
#define ISIZE	6
#define TSIZE	144
#define BIGFSIZ	24
#define LITFSIZ	12
#define NPRINT	100

/*
 *  header for DEM tapes, after translation (original is ascii)
 */
typedef struct {
	char            title[TSIZE];
	double          delx;
	double          dely;
	double          delz;
	double          profdir;
	double          tcons[15];
	double          northing[4];
	double          easting[4];
	double          zmax;
	double          zmin;
	int             accdata;
	int             demlevl;
	int             elevpatt;
	int             elevunits;
	int             ncols;
	int             nprofls;
	int             nrows;
	int             polysides;
	int             refsys;
	int             refunits;
	int             zone;
}               DEM_HDR;

/*
 *  profile record, after translation (original is ascii)
 */
typedef struct {
	int             rownum;		/* (rownum, 1) or (1, colnum)	 */
	int             colnum;		/* (rownum, 1) or (1, colnum)	 */
	int             ni;		/* # elevs (1, nj) or (ni, 1)	 */
	int             nj;		/* # elevs (1, nj) or (ni, 1)	 */
	double          elevdatum;	/* added to elevations	 */
	double          northing;	/* Northing coord of 1st pt	 */
	double          easting;	/* Easting coord of 1st pt	 */
	double          zmax;		/* max elev in profile	 */
	double          zmin;		/* min elev in profile	 */
}               PROF_HDR;

 /*
  * that part of the profile header that is needed to retrieve
  * elevations
  */
typedef struct {
	double          northing;	/* Northing coord	 */
	double          easting;	/* Easting coord	 */
	int             num;		/* profile number	 */
	int             nelev;		/* # elevations in profile	 */
}               DEM_SAVHDR;

 /*
  * template of Record A on DEM tape
  */
typedef struct {
	char            filename[TSIZE];
	char            levlcode[ISIZE];
	char            elevpatt[ISIZE];
	char            refsys[ISIZE];
	char            zone[ISIZE];
	char            proj_param[15][BIGFSIZ];
	char            refunits[ISIZE];
	char            elevunits[ISIZE];
	char            polysides[ISIZE];
	char            corners[4][2][BIGFSIZ];
	char            elevmin[BIGFSIZ];
	char            elevmax[BIGFSIZ];
	char            rotate[BIGFSIZ];
	char            acc_code[ISIZE];
	char            delx[LITFSIZ];
	char            dely[LITFSIZ];
	char            delz[LITFSIZ];
	char            rownum[ISIZE];
	char            colnum[ISIZE];
}               A_RECORD;

 /*
  * template of Record B on DEM tape
  */
typedef struct {
	char            rowid[ISIZE];
	char            colid[ISIZE];
	char            nrows[ISIZE];
	char            ncols[ISIZE];
	char            xgo[BIGFSIZ];
	char            ygo[BIGFSIZ];
	char            elevdatum[BIGFSIZ];
	char            elevmin[BIGFSIZ];
	char            elevmax[BIGFSIZ];
}               B_RECORD;

/*
 * external parameters for demx program
 */

typedef struct {
	FILE           *i_fp;		/* input image stream ptr	 */
	int             h_fd;		/* LQH file descriptor		 */
	int             o_fd;		/* output image file descriptor	 */
	DEM_SAVHDR     *i_sav;		/* array of saved DEM_SAVHDR's	 */
	bool_t          raw;		/* ? raw values, not LQH	 */
	bool_t          verbose;	/* ? verbose output		 */
	bool_t          cmdset;		/* ? range set from cmd line	 */
	bool_t          islqh;		/* ? input LQH specified	 */
	char           *csys;		/* coordinate system		 */
	char           *ftemp;		/* file to store first pass	 */
	char           *runits;		/* datum units			 */
	char           *units;		/* units of elevation		 */
	double          delx;		/* spacing between cols		 */
	double          dely;		/* spacing between rows		 */
	double          emax;		/* maximum easting		 */
	double          emin;		/* minimum easting		 */
	double          nmax;		/* maximum northing		 */
	double          nmin;		/* minimum northing		 */
	fpixel_t        fillval;	/* fill for out-of-bounds pixels */
	fpixel_t        zmax;		/* maximum elevation		 */
	fpixel_t        zmin;		/* minimum elevation		 */
	int             maxelevs;	/* max size of profile		 */
	int             nprofls;	/* # profiles in DEM		 */
}               PARM_T;

extern PARM_T   parm;

extern PROF_HDR	*profhdr(DEM_SAVHDR *s);
extern char    *proj(int refsys);
extern char    *units(int refunits);
extern double   bigatof(char *s);
extern double   litatof(char *s);
extern int      atoid(char *s);
extern void     demhdr(void);
extern void     headers(void);
extern void     output(void);
extern void     prdemhdr(DEM_HDR *dhdr);
extern void     prprofhdr(PROF_HDR *ph);
extern void	profiles(void);
extern void     fillvec(REG_1 fpixel_t *x, FREG_1 fpixel_t fill, REG_2 int n);

#endif
