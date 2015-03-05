
#include "ipw.h"
#include "pgm.h"
#include "mapproj.h"

int
main(
	int             argc,
	char          **argv)
{
	static OPTION_T opt_a = {
		'a', "semi-major axis",
		REAL_OPTARGS, "smajorax",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_c = {
		'c', "scale factor; (HOM only) followed by origin latitude",
		REAL_OPTARGS, "scale",
		OPTIONAL, 1, 2
	};

	static OPTION_T opt_d = {
		'd', "datum code -- default: 0 (Clarke 1866)",
		INT_OPTARGS, "datum",
		OPTIONAL, 1, 1
        };

	static OPTION_T opt_e = {
		'e', "eccentricity squared; OR semi-minor axis",
		REAL_OPTARGS, "eccsq",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_f = {
		'f', "Landsat end-of-path flag",
		INT_OPTARGS, "flag",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_h = {
		'h', "height of perspective point",
		REAL_OPTARGS, "height",
		OPTIONAL, 1, 1
        };

	static OPTION_T opt_i = {
		'i', "inclination of orbit",
		REAL_OPTARGS, "inclin",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_l = {
		'l', "longitude (+E/-W) and latitude (+N/-S), decimal degrees format",
		REAL_OPTARGS, "deg",
		OPTIONAL, 1, 4
	};

	static OPTION_T opt_n = {
		'n', "zone code (UTM or State Plane Coordinate System)",
		INT_OPTARGS, "zone",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_o = {
		'o', "offset of upper-left corner from origin (false-east,north)",
		REAL_OPTARGS, "offset",
		OPTIONAL, 2, 2
	};

	static OPTION_T opt_p = {
		'p', "projection type (see documentation for list of types)",
		STR_OPTARGS, "projec",
		REQUIRED, 1, 1
	};

	static OPTION_T opt_r = {
		'r', "radius of sphere of reference (in SOM only: Landsat ratio)",
		REAL_OPTARGS, "radius/ratio",
		OPTIONAL, 1, 1
        };

	static OPTION_T opt_s = {
		's', "standard parallel(s)",
		REAL_OPTARGS, "stlat",
		OPTIONAL, 1, 2
	};

	static OPTION_T opt_t = {
		't', "orbital period, in minutes",
		REAL_OPTARGS, "period",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_u = {
		'u', "code number for units used -- default: 2 (meters)",
		INT_OPTARGS, "unit-code",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_w = {
		'w', "Landsat path number (Landsats 1, 2, 3)",
		INT_OPTARGS, "path#",
		OPTIONAL, 1, 1
	};

	static OPTION_T opt_z = {
		'z', "azimuth of centerline, longitude of centerline at meridian",
		REAL_OPTARGS, "az/long",
		OPTIONAL, 2, 2
	};

	static OPTION_T *optv[] = {
		&opt_a,
                &opt_c,
		&opt_d,
		&opt_e,
		&opt_f,
		&opt_h,
		&opt_i,
		&opt_l,
		&opt_n,
		&opt_o,
		&opt_p,
		&opt_r,
		&opt_s,
		&opt_t,
		&opt_u,
                &opt_w,
		&opt_z,
		0
	};

	/*
	 *  List of projection-specific options
	 */
	static OPTION_T *proj_opts[] = {
		&opt_a,
		&opt_c,
		&opt_e,
		&opt_f,
		&opt_h,
		&opt_i,
		&opt_l,
		&opt_n,
		&opt_o,
		&opt_r,
		&opt_s,
		&opt_t,
		&opt_w,
		&opt_z,
		0
	};

	/*
	 *  Table of options' statuses for each projection type
	 *
	 *	character 'R' = required option
	 *	    "     '?' = optional option
	 *          "     '.' = illegal  option
	 */
	static char *opt_status[] = {
		/* name 	options: acefhilnorstwz		*/
		/* geo    =  0 */	".............." ,
		/* utm    =  1 */	"......??......" ,
		/* spcs   =  2 */	".......R......" ,
		/* albers =  3 */	"?.?...R.?.?..." ,
		/* lamcc  =  4 */	"?.?...R.?.?..." ,
		/* mercat =  5 */	"?.?...R.?....." ,
		/* ps     =  6 */	"?.?...R.?....." ,
		/* polyc  =  7 */	"?.?...R.?....." ,
		/* equidc =  8 */	"?.?...R.?.R..." ,
		/* tm     =  9 */	"???...R.?....." ,
		/* stereo = 10 */	"......R.??...." ,
		/* lamaz  = 11 */	"......R.??...." ,
		/* azmeqd = 12 */	"......R.??...." ,
		/* gnomon = 13 */	"......R.??...." ,
		/* ortho  = 14 */	"......R.??...." ,
		/* gvnsp  = 15 */	"....R.R.??...." ,
		/* snsoid = 16 */	"......R.??...." ,
		/* eqrect = 17 */	"......R.??...." ,
		/* miller = 18 */	"......R.??...." ,
		/* vgrint = 19 */	"......R.??...." ,
		/* hom    = 20 */	"???...?.?....?" ,
		/* som    = 21 */	"?.??.??.??.??." ,
					0
	};


	short		proj_id;	/* id of given projection type	 */
	char	       *proj_name;	/* name of given projection type */
	struct projdef  proj; 		/* projection def. parameters    */
	short           i;
	OPTION_T       **opt;		/* -> project-specific options   */
	char	       *status;		/* status of project-spec option */

	bool_t	       got_axis;	/* flag if semi-major axis spec. */
	bool_t         got_eccsq ;      /* flag if eccsq is specified    */
	double	       eccsq;		/* eccentricity squared value	 */
	double         orbit_incl ;     /* orbit inclination             */
	double         orbit_per ;      /* orbit period                  */
	double         orbit_path ;     /* Landsat path number           */

	short          unitcode ;       /* units for projection          */
	short          zonecode ;       /* zone code, for UTM or SPCS    */
	double         azim[2] ;        /* azimuth angle. longitude      */
	double         height ;         /* height of perspective         */
	double         long_lat[4] ;    /* longitude and latitude args   */
	double         radius ;         /* raduis of sphere of reference */
	double         scale[2] ;       /* scale factor,latitude of orig */
	double	       smajorax;	/* semi-major axis		 */
	double	       stdparl[2];	/* standard parallels		 */

/*
 * initialize variables
 */
	zonecode = 0 ;
	got_axis = FALSE ;
	got_eccsq = FALSE ;
	scale[0] = 1.0 ;
/*
 *  The following orbital parameters are for Landsats 1,2,3 and are taken
 *  from the USGS LAS documentation for their program "projon"
 */
	orbit_incl = 099005031.2 ;
	orbit_per = 103.2669323 ;
	orbit_path = 0 ;

/*
 * begin
 */
	ipwenter(argc, argv, optv, IPW_DESCRIPTION);

/*
 * check for known projection type
 */
	proj_id = get_proj_id(str_arg(opt_p,0));
	if (proj_id == UNKNOWN) {
		error("unknown projection type: %s",str_arg(opt_p,0)) ;
	}
	proj_name = get_proj_name(proj_id);

/*
 * initialization projection definition
 */
	proj.id = proj_id ;
	proj.uid = METERS ;
	proj.zone = UNDEF_ZONE;
	proj.datum = 0;
	for (i=0; i < NPARMS ; i++)
		proj.parms[i] = 0.0 ;

/* 
 * Check for required and illegal options for the given projection type
 */
	for (opt = proj_opts, status = opt_status[proj_id];
				*opt != NULL;
				opt++, status++) {
	    if ((*status == 'R') && (! got_opt(**opt))) {
		error("projection \"%s\" requires option \"-%c\"", proj_name,
		      (*opt)->option);
	    }
	    else if ((*status == '.') && got_opt(**opt)) {
		error("option \"-%c\" is illegal for projection \"%s\"",
		      (*opt)->option, proj_name);
	    }
	}


/*
 * set defaults for specific projection types
 */
	switch (proj_id) {
	    case ALBERS :
	    case EQUIDC :
		proj.parms[2] = pack_dms(29.5) ;
		proj.parms[3] = pack_dms(45.5) ;
		break;

	    case LAMCC :
		proj.parms[2] = pack_dms(33.0) ;
		proj.parms[3] = pack_dms(45.0) ;
		break;

	    case TM :
		proj.parms[2] = 1 ;
		break;

	    case STEREO :
	    case LAMAZ :
	    case AZMEQD :
	    case GNOMON :
	    case ORTHO :
	    case GVNSP :
	    case SNSOID :
	    case EQRECT :
	    case MILLER :
	    case VGRINT :
		proj.parms[0] = 6370997 ;
		break;

	    case SOM :         /* from USGS LAS projon program, table C */
		proj.parms[3] = 099005031.2 ;
		proj.parms[4] = 128.87 ;
		proj.parms[8] = 103.2669323 ;
		proj.parms[9] = 0.5201613 ;
		break;
	}

/*
 * check for those projections which require at least one of a pair of options.
 */
	switch (proj_id) {
	    case HOM :
		opt_check(1, 1, 2, &opt_l, &opt_z);
		break;

	    case SOM :
		opt_check(1, 1, 2, &opt_l, &opt_w);
		break;

	    case UTM :
		opt_check(1, 1, 2, &opt_l, &opt_n);
		break;
	}

/*
 * check individual command line arguments, and set appropriate values in the
 * projection definition stucture
 */

    /*
     * option -a
     */
	if (got_opt(opt_a)) {
		smajorax = real_arg (opt_a, 0);
		if (smajorax <= 0.0)
			error ("Illegal semi-major axis: %f", smajorax);
	  	got_axis = TRUE ;
	  	proj.parms[0] = smajorax ;
	}

    /*
     * option -c
     */
	if (got_opt(opt_c)) {
		scale[0] = real_arg(opt_c,0);
		if (scale[0] <= 0.0)
	    	    error("illegal scale factor for %s : %f",
			  proj_id == TM ? "central meridian" :
					  "center of projection" ,
		          scale[0]) ;
		proj.parms[2] = scale[0] ;

		if (proj_id == HOM) {
	    	    if (opt_c.nargs < 2)
	    		error("option \"-c\": missing latitude for projection origin");
	    	scale[1] = real_arg(opt_c,1) ;
	    	if ((scale[1] < -90.0) || (scale[1] > 90.0))
	      	    error("illegal latitude for projection origin: %f",
			  scale[1]) ;
	        proj.parms[5] = pack_dms(scale[1]) ;
	        }
		else {  /* not HOM projection */
		    if (opt_c.nargs > 1)
		       error("option \"-c\" for projection %s requires 1 value",
			     proj_name);
		}
	}

    /*
     * option -e
     */
	if (got_opt(opt_e)) {
	    if (! got_axis)
		error ("must specify semi-major axis if using eccentricity squared");
	    eccsq = real_arg (opt_e, 0);
	    if (eccsq < 0.0)
	    	error ("Illegal eccentricity squared: %f", eccsq);
	    got_eccsq = TRUE ;
	    proj.parms[1] = eccsq ;
	}

    /*
     * option -d
     */
	if (got_opt(opt_d)) {
	    if (got_axis) {
		if (got_eccsq) {
	      	    warn("axis and eccentricity specified: datum will be ignored") ;
	        }
	        else {
	      	    warn("axis specified: datum will be ignored, eccsq set to 0.0") ;
	      	    eccsq = 0.0 ;
	            proj.parms[1] = 0.0 ;
	        }
	    }
	    else {
	    	proj.datum = int_arg (opt_d, 0);
	    	if (proj.datum < 0 || proj.datum >= NDATUM) 
	    	    error ("Illegal datum code: %d", proj.datum);
	    }
	}

    /*
     * option -f
     */
	if (got_opt(opt_f)) {
		proj.parms[10] = int_arg(opt_f,0);
	}
	    
    /*
     * option -h
     */
	if (got_opt(opt_h)) {
	    height = real_arg(opt_h,0) ;
	    if (height <= 0.0)
		error("illegal value for height of perspective: %f",height) ;
	    proj.parms[2] = height ;
	}

	    
    /*
     * option -i
     */
	if (got_opt(opt_i)) {
	    orbit_incl = real_arg(opt_i,0) ;
	    if((orbit_incl < (-180.0)) || (orbit_incl > 360.0))
		error("illegal value for orbit inclination: %f",orbit_incl) ;
	    proj.parms[3] = pack_dms(orbit_incl) ;
	}

    /*
     * option -l
     */
	if (got_opt(opt_l)) {
	  long_lat[0] = real_arg(opt_l,0) ;
	  switch (proj_id) {
	    case SNSOID :
	    case MILLER :
	    case SOM :
		if (long_lat[0] < -180.0 || long_lat[0] > 180.0)
			error("illegal longitude for %s : %f",
			      proj_id == SOM ? "ascending node of orbit"
					     : "central meridian" ,
		              long_lat[0]) ;
		proj.parms[4] = pack_dms(long_lat[0]) ;
		if (opt_l.nargs > 1)
		       error("option \"-l\" for projection %s requires 1 value",
			     proj_name);
		break;

	    case HOM :
		if (opt_l.nargs < 4)
		    error("option \"-l\" for projection %s requires 4 values",
			  proj_name);
		if (long_lat[0] < -180.0 || long_lat[0] > 180.0)
		    error("illegal longitude for first point: %f", long_lat[0]);
		long_lat[1] = real_arg(opt_l,1) ;
		if (long_lat[1] < -90.0 || long_lat[1] > 90.0)
		    error("illegal latitude for first point: %f", long_lat[1]);
		long_lat[2] = real_arg(opt_l,2) ;
		if (long_lat[2] < -180.0 || long_lat[2] > 180.0)
		    error("illegal longitude for second point: %f",
			  long_lat[2]) ;
	        long_lat[3] = real_arg(opt_l,3) ;
		if (long_lat[3] < -90.0 || long_lat[3] > 90.0)
		    error("illegal latitude for second point: %f", long_lat[3]);
		proj.parms[8] = pack_dms(long_lat[0]) ;
		proj.parms[9] = pack_dms(long_lat[1]) ;
		proj.parms[10] = pack_dms(long_lat[2]) ;
		proj.parms[11] = pack_dms(long_lat[3]) ;
		break;

	    default :
		if (long_lat[0] < -180.0 || long_lat[0] > 180.0)
	 	    error("illegal longitude: %f", long_lat[0]) ;
		long_lat[1] = real_arg(opt_l,1) ;

		if (opt_l.nargs != 2)
		     error("option \"-l\" for projection %s requires 2 values",
			   proj_name);
		if (long_lat[1] < -90.0 || long_lat[1] > 90.0)
		     error("illegal latitude: %f", long_lat[1]) ;

		if (proj_id == UTM) {
			proj.parms[0] = pack_dms(long_lat[0]) ;
			proj.parms[1] = pack_dms(long_lat[1]) ;
		}
		else {
			proj.parms[4] = pack_dms(long_lat[0]) ;
			proj.parms[5] = pack_dms(long_lat[1]) ;
		}
	  } /* switch */ 
	} /* if */
	    
    /*
     * option -n
     */
	if (got_opt(opt_n)) {
	  zonecode = int_arg(opt_n,0) ;
	  if (proj_id == UTM) {
	    if (got_opt (opt_l))
	      error("use option \"-l\" or \"-n\" with projection %s",
		    proj_name);
	    if (zonecode < -NUTMZ || zonecode > NUTMZ || zonecode == 0)
	      error("illegal UTM zone code: %d",zonecode) ;
	    proj.zone = zonecode ;
	    if (zonecode < 0) {              /* Southern Hemisphere */
	      proj.parms[6] = 100000000 ;
	      proj.parms[7] = 500000 ;
	    }
	  } 
	  else if (proj_id == SPCS) {
	    if (zonecode < MIN_SPZ || zonecode > MAX_SPZ)
	      error("illegal SPCS zone code: %d",zonecode) ;
	    proj.zone = zonecode ;
	  }
	}

    /*
     * option -o
     */
	if (got_opt(opt_o)) {
	  proj.parms[6] = real_arg(opt_o,0) ;
	  proj.parms[7] = real_arg(opt_o,1) ;
	}

    /*
     * option -r
     */
	if (got_opt(opt_r)) {
	  if (proj_id == SOM) {
	    proj.parms[9] = real_arg(opt_r,0) ;
	  }
	  else {
	    radius = real_arg(opt_r,0) ;
	    if (radius <= 0)
	      error("illegal radius for reference sphere: %f",radius) ;
	    proj.parms[0] = radius ;
	  } 
	}

    /*
     * option -s
     */
	if (got_opt(opt_s)) {
	  stdparl[0] = real_arg(opt_s,0) ;
	  if (stdparl[0] < -90.0  || stdparl[0] > 90.0)
	    error("illegal value for standard parallel: %f",stdparl[0]) ;
	  proj.parms[2] = pack_dms(stdparl[0]) ;

	  if (opt_s.nargs > 1) {
	    stdparl[1] = real_arg (opt_s, 1);
	    if (stdparl[1] < -90.0 || stdparl[1] > 90.0)
	      error ("illegal value for second standard parallel: %f",
		     stdparl[1]);
	    proj.parms[3] = pack_dms(stdparl[1]) ;
	    if (proj_id == EQUIDC)
	      proj.parms[8] = TRUE ;
	  }
	  else {
		if (proj_id != EQUIDC)
		     error("option \"-s\" for projection %s requires 2 values",
			   proj_name);
	  }
	}
 
    /*
     * option -t
     */
	if (got_opt(opt_t)) {
	  orbit_per = real_arg(opt_t,0) ;
	  if (orbit_per <= MIN_PER)
	    error("orbit period too short: must be %d minutes or more",
		  MIN_PER);
	  proj.parms[8] = orbit_per ;
	}

    /*
     * option -u
     */
	if (got_opt(opt_u)) {
	  unitcode = int_arg(opt_u,0) ;
	  if(unitcode < 0 || unitcode >= NUNITS)
	    error("illegal units code: must be in range 0-%d",NUNITS-1) ;
	  switch(proj_id) {
/*
 * projections that must use angular units (degrees, radians, arc sec.,  etc.)
 */
	    case GEO : if(unitcode == 1 || unitcode == 2)
	               error("illegal units code: %d -- must be angular units (0,3,4, or 5)",unitcode) ;
	             break ;
/*
 * projections that must use linear units (feet, meters, miles, etc.)
 */
	    case UTM :
	    case SPCS :
	    case ALBERS :
	    case TM :
	    case LAMAZ:
	    case AZMEQD:
	    case ORTHO:
	    case EQRECT:
	    case HOM:
	    case SOM: if( unitcode != 1 && unitcode != 2)
	               error("illegal units code: %d -- must be linear units (1 or 2)",unitcode) ;
                     break ;
	  }
	  proj.uid = unitcode ;
	}

    /*
     * option -w
     */
	if (got_opt(opt_w)) {
	  orbit_path = int_arg(opt_w,0) ;
	  if (orbit_path < 0)
	    error("illegal Landsat path number: %d",orbit_path) ;
	  /*
	   *  The formula below is correct for Landsat-1, 2, 3 orbits;
	   *  however, it's different for Landsat-4, 5.
	   *  J. Domingo, OSU, 5 June 1996
	   */
	  proj.parms[4] = pack_dms(128.87 - (360/251 * orbit_path)) ;
	}

    /*
     * option -z
     */
	if (got_opt(opt_z)) {
	  azim[0] = real_arg(opt_z,0) ;
	  if (azim[0] < -180.0 || azim[0] > 360.0)
	    error("illegal azimuth angle: %f",azim[0]) ;
	  proj.parms[3] = pack_dms(azim[0]) ;

	  azim[1] = real_arg(opt_z,1) ;
	  if (azim[1] < -180.0 || azim[1] > 360.0)
	    error("illegal longitude for azimuth point: %f",azim[1]) ;
	  proj.parms[4] = pack_dms(azim[1]) ;
	  proj.parms[12] = TRUE ;
	}

   /* write parameters to file */

	printf ("%d\n", proj.id);
	printf ("%d\n", proj.uid);
	printf ("%d\n", proj.zone);
	printf ("%d\n", proj.datum);

	for (i = 0; i < NPARMS; i++) {
		printf ("%.*g\n", DBL_DIG, proj.parms[i]);
	}

	ipwexit(EXIT_SUCCESS);
}
