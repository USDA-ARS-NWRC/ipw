PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='nz:w:g:s:x:d:m:q:k:'
synopsis='[-n] -z elev -w omega -g gfact [-s S0] [-x w1,w2] [-m mask] \
	  [-k rad_prefix] -d y,m,d [-q npts] [image]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $pgm "$synopsis" "$description"
}

elev=
omega=
gfact=
S0=
wrange=
year=
month=
day=
date=
mask=
net=0
keep_rad=0
nquad=21

# parse options, if none print description

case $# in
0|1)	exec usage $pgm "$synopsis" "$description"
	;;
esac

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	exec usage $pgm "$synopsis" "$description"
		;;
	-n)	net=1
		;;
	-z)	elev=$2
		shift
		;;
	-w)	omega=$2
		shift
		;;
	-g)	gfact=$2
		shift
		;;
	-s)	S0=$2
		shift
		;;
	-x)	wrange=$2
		shift
		;;
	-d)	date=$2
		shift
		;;
	-m)	mask=$2
		shift
		;;
	-k)	keep_rad=1
		rad_prefix=$2
		shift
		;;
	-q)	nquad=$2
		shift
		;;
	*)	exec sherror $pgm '"getopt" failed'
		;;
	esac
	shift
done

tdir=$TMPDIR/$pgm.$$
 
trap 'rm -f -r $tdir' 0
trap 'exit 0' 1 2 3 15
 
mkdir $tdir

# list of temporary files used (i.. image; a.. ascii)
# directory for all is $TMPDIR/$pgm.$$
#
# iX - copy of input if stdin
# iZ - elevation image (1 band)
# iGR - gradient image (2 bands)
# iVF - view factor/albedo image (3 bands)
# iR0 - reflectance of substrate image (2 bands)
# iOD - optical depth image (2 bands)
# iAZ.n - weights, solar azimuth/zenith images output from sunweights (21 images)
# aS - input for final awk script
# sh - sequence of commands to run
#
# the rest of the files are named within awk script
#
# wrad.xx - weighted interval radiation files to be summed by iadd
#
# the following files are re-created for each time step
#
# shade - cosine illumination angle, corrected for horizons
# hor - horizon mask
# erad - beam and diffuse radiation over elevation grid

# Can only have 1 input image.  If stdin must make duplicate copy
# because we need multiple access.

img=$1
case $# in
0)	test -t 0 && {
	exec sherror $pgm "can't read image data from terminal"
	}
	;;
1)	;;
*)	exec usage $pgm "$synopsis" "$description"
	;;
esac

case $img in
''|'-')
	cat $img > $tdir/iX
	image=$tdir/iX
	;;
*)	test -r $img || {
		exec sherror $pgm "can't open file", $image
	}
	image=$img
	;;
esac

# can't write image data to terminal

test -t 1 && {
	exec sherror $pgm "can't write image data to a terminal"
}

# parse date into year, month, day

case $date in
'')	exec usage $pgm "$synopsis" "$description"
	;;
*)	year=` echo $date | awk -F, '{print $1}'`
	month=`echo $date | awk -F, '{print $2}'`
	day=`  echo $date | awk -F, '{print $3}'`
	;;
esac

# make sure all essential arguments in

case $elev in
'')	exec sherror $pgm "-z arg missing"
	;;
esac

case $omega in
'')	exec sherror $pgm "-w arg missing"
	;;
esac

case $gfact in
'')	exec sherror $pgm "-g arg missing"
	;;
esac

case $S0 in
'')	case $wrange in
	'')	exec sherror $pgm "-s and -x args missing, one must be present"
		;;
	*)	S0=`solar -d $date -w $wrange -a`
		;;
	esac
esac

case $year in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $month in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $day in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $mask in
'')	maskf=
	;;
*)	test -r $mask || {
		exec sherror $pgm "can't open file", $mask
	}
	maskf="-m $mask"
esac

case $nquad in
15)	;;
21)	;;
*)	exec sherror $pgm "-q option must be 15 or 21"
	;;
esac

if [ $keep_rad -eq 0 ] ; then
	rad_prefix=$tdir/rad
fi


# elevation file
demux -b 0 $image > $tdir/iZ

# slope/aspect file
demux -b 1,2 $image > $tdir/iGR

# sky view, terrain view, and albedo
demux -b 3,4,5 $image > $tdir/iVF

# reflectance of substrate and optical depth
demux -b 6 $image > $tdir/iR0
demux -b 7 $image > $tdir/iOD

# 1st awk script writes weights to temporary file and sun angles to temp images

sunweights -d $date -q $nquad -o $tdir/iAZ $maskf $tdir/iZ

# 2nd awk script uses these values to create processing script for gtoporad

echo $net $nquad $elev $omega $gfact $S0 \
	$tdir/iZ $tdir/iGR $tdir/iVF $tdir/iR0 $tdir/iOD $tdir/iAZ $tdir/ \
	$keep_rad $rad_prefix $maskf \
	> $tdir/aS

cat $tdir/aS | awk '
{
	if ($1 == 0)
		gtoporad = "gtoporad -b 8"
	else
		gtoporad = "gtoporad -b 8 -n"
	nquad = $2
	elev = " -z " $3
	omega = " -w " $4
	gfact = " -g " $5
	S0 = " -s " $6
	efile = $7
	grfile = $8
	vfile = $9
	R0file = $10
	ODfile = $11
	azprefix = $12
	froot = $13
	keep_rad = $14
	rad_prefix = $15
	maskf = " " $16 " " $17
	ecmd = " gelevrad -n 8,8" elev omega gfact S0 maskf

	for (j = 0; j < nquad; j++) {
		azfile = sprintf("%s.%02d", azprefix, j);
		wfile = sprintf("%sweights", froot);
		print "demux -b 0", azfile, ">", wfile
		solfile = sprintf("%ssolar", froot)
		print "demux -b 1,2", azfile, ">", solfile
		mufile = sprintf("%smu", froot);
		print "demux -b 0", solfile, ">", mufile
		rfile = sprintf("%serad", froot);
		print "mux ", efile, mufile, R0file, ODfile, "|\\"
		print ecmd, ">", rfile
		hfile = sprintf("%shor", froot);
		print "ghorizon -b -s", solfile, maskf, efile, " >", hfile
		sfile = sprintf("%sshade", froot);
		print "gshade -s", solfile, "-i", grfile, maskf, "|"
		print "	mux", "-", hfile, "|"
		print "	bitcom -m -a >",sfile
		radfile = sprintf("%s.%02d", rad_prefix, j);
		print "mux", mufile, "\\"
		print "	", rfile, "\\"
		print "	", sfile, "\\"
		print "	", vfile, "\\"
		print "	", R0file, "|"
		print "	", gtoporad, " >", radfile
		tfile[j] = sprintf("%swrad.%02d", froot, j);
		print "mux", wfile, "\\"
		print " ", radfile, "| mult -n 8 >", tfile[j]
		print ""
	}
}
END {
	n = j
	print "mux", "\\"
	for (j = 0; j < n; ++j)
		print "	", tfile[j], "\\"
	ORS = " "
	print "| iadd"
}' > $tdir/sh$$

# run command string from temporary file

chmod +x $tdir/sh$$
sh -c $tdir/sh$$

# remove temporary files

rm -rf $tdir

exit 0
