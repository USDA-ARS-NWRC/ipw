PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='nz:t:w:g:x:s:d:f:y:b:l:a:m:c:r:k:A:D'
synopsis='[-n] [-z elev] [-t tau] [-w omega] [-g gfact] -x w1,w2 \
	-s start -d day -f zone -y year	[ -b d,m,s -l d,m,s ] [ -A mu,azm ] \
	-a gsize -m maxgsz -c dirt [-r erad_image] [-k alb_image] [-D] [image]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $pgm "$synopsis" "$description"
}

elev=
tau=
omega=
gfact=
R0=
S0=
wrange=
year=
month=
day=
hour=0
min=0
sec=0
date=
lat=
lon=
mu_azm=
net=
gsize=
maxgsz=
dirt=
start=
wyd=
wy=
zone=
rfile=
keep_alb=0
afile=
diffuse=

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
	-n)	net="-n"
		;;
	-z)	elev=$2
		shift
		;;
	-t)	tau=$2
		shift
		;;
	-w)	omega=$2
		shift
		;;
	-g)	gfact=$2
		shift
		;;
	-x)	wrange=$2
		shift
		;;
	-s)	start=$2
		shift
		;;
	-d)	wyd=$2
		shift
		;;
	-f)	zone="-g $2"
		shift
		;;
	-y)	wy=$2
		shift
		;;
	-b)	lat=$2
		shift
		;;
	-l)	lon=$2
		shift
		;;
	-A)	mu_azm=$2
		shift
		;;
	-a)	gsize=$2
		shift
		;;
	-m)	maxgsz=$2
		shift
		;;
	-c)	dirt=$2
		shift
		;;
	-r)	rfile=$2
		shift
		;;
	-k)	afile=$2
		keep_alb=1
		shift
		;;
	-D)	diffuse="-d"
		;;
	*)	exec sherror $pgm '"getopt" failed'
		;;
	esac
	shift
done

# list of temporary files used
# directory for all is $TMPDIR/$pgm.$$
#
# iX - copy of input if stdin
# iZ - elevation image (1 band)
# iGR - gradient image (2 bands)
# iVF - view factor image (2 bands)
#
# alb - albedo image (1 band)
# shade - cosine illumination angle image, corrected for horizons (1 band)
# hor - horizon mask (1 band)
# erad - beam and diffuse radiation over elevation grid (2 bands)

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

if [ -n "$rfile" ] ; then
	if [ ! -r $rfile ] ; then
		exec sherror $pgm "$rfile not found"
	fi
fi


# make sure all essential arguments in

if [ -z "$rfile" ] ; then
	case $elev in
	'')	exec sherror $pgm "-z arg missing"
		;;
	esac

	case $tau in
	'')	exec sherror $pgm "-t arg missing"
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

	case $wrange in
	'')	exec sherror $pgm "-x arg missing"
		;;
	esac
fi

case $start in
'')	exec sherror $pgm "-s arg missing"
	;;
esac

case $wy in
'')	exec sherror $pgm "-y arg missing"
	;;
esac

case $wyd in
'')	exec sherror $pgm "-d arg missing"
	;;
esac

case $gsize in
'')	exec sherror $pgm "-a arg missing"
	;;
esac

case $maxgsz in
'')	exec sherror $pgm "-m arg missing"
	;;
esac

case $dirt in
'')	exec sherror $pgm "-c arg missing"
	;;
esac


#  Either -b lat and -l lon specified, or -A mu,azm

if [ -z "${lat}${lon}${mu_azm}" ] ; then
	exec sherror $pgm "Either -b and -l options, or -A option required"
fi

if [ -z "$mu_azm" ] ; then
	case $lat in
	'')	exec sherror $pgm "-b arg missing or incomplete"
		;;
	esac

	case $lon in
	'')	exec sherror $pgm "-l arg missing or incomplete"
		;;
	esac
else
	if [ -n "$lat" ] ; then
		exec sherror $pgm "-b and -A options cannot be used together"
	fi

	if [ -n "$lon" ] ; then
		exec sherror $pgm "-l and -A options cannot be used together"
	fi
fi


# convert water year day into year, month, day, hour, min, sec

date=`echo $wyd | cvtime -i wy -o date -y $wy $zone`
year=` echo $date | awk -F, '{print $1}'`
month=`echo $date | awk -F, '{print $2}'`
day=`  echo $date | awk -F, '{print $3}'`
hour=` echo $date | awk -F, '{print $4}'`
min=`  echo $date | awk -F, '{print $5}'`
sec=`  echo $date | awk -F, '{print $6}'`

# Make sure requested wavelengths do not cross IR/vis boundary

wflag=`echo $wrange | awk -F, '
BEGIN {
	v_min=.28
	v_max=.7
	ir_min=.7
	ir_max=2.8
}
{
	if ($1 >= ir_min && $2 <= ir_max) 
		printf "-r" 
	else if ($1 >= v_min && $2 <= v_max) 
		printf "-v"
}'`

if [ -z "$wflag" ] ; then
	exec sherror $pgm "Illegal wavelength range $wrange (IR or vis only)"
fi

# Convert wavelengths to exoatmospheric solar irradiance

S0=`solar -d $year,$month,$day -w $wrange -a` || exit 1


tdir=$TMPDIR/$pgm.$$
mkdir $tdir

case $img in
''|'-')
	cat $img > $tdir/iX
	image=$tdir/iX
	;;
*)	test -r $img || {
		exec sherror $pgm "can't open file" $img
	}
	image=$img
	;;
esac

# can't write image data to terminal

test -t 1 && {
	exec sherror $pgm "can't write image data to a terminal"
}


# elevation file
efile=$tdir/iZ
demux -b 0 $image > $efile

# slope/aspect file
grfile=$tdir/iGR
demux -b 1,2 $image > $grfile

# sky view and terrain view
vfile=$tdir/iVF
demux -b 3,4 $image > $vfile

# horizon output file
hfile=$tdir/hor

# shade output file
sfile=$tdir/shade

# ialbedo output file
if [ $keep_alb -eq 0 ] ; then
	afile=$tdir/alb
fi


#   Get sun angles for this time (if not specified)

if [ -z "$mu_azm" ] ; then
	angout=`sunang -t $date -b $lat -l $lon`
	mu=`expr "$angout" : '.*-u \(.*\) -a'`
	azm=`expr "$angout" : '.*-a \(.*\)'`
else
	mu=`expr "$mu_azm" : '\(.*\),'`
	azm=`expr "$mu_azm" : '.*,\(.*\)'`
	if [ -z "$mu" ] ; then
		exec sherror $pgm "Missing mu arg for -A option"
	fi
	if [ -z "$azm" ] ; then
		exec sherror $pgm "Missing azm arg for -A option"
	fi
fi

sun_down=`echo $mu | grep -c "-"`

# If sun below horizon print message (don't run models)

if [ $sun_down -ne 0 ] ; then

	echo Sun below horizon at $wyd \($date\)

else

# Run horizon to get sun-below-horizon mask

	horizon -u $mu -a $azm $efile > $hfile

# Run shade to get cosine local illumination angle; mask by horizon mask

	shade -u $mu -a $azm $grfile | \
			mux - $hfile | \
			bitcom -m -a > $sfile

# Run ialbedo to get albedo
	if [ -f $start ];
	then
		ialbedo -d $wyd -g $gsize -m $maxgsz -c $dirt -i $start $wflag $sfile > $afile
	else
		ialbedo -s $start -d $wyd -g $gsize -m $maxgsz -c $dirt $wflag $sfile > $afile
	fi

# Run imgstat to get R0: mean albedo

	R0=`imgstat $afile | awk '/mean/ {print $2}'`

# Run elevrad to get beam & diffuse (if -r option not specified)

	if [ -z "$rfile" ] ; then
		rfile=$tdir/erad
		elevrad -n 8 -z $elev -t $tau -w $omega -g $gfact -r $R0 -s $S0 \
			-u $mu $tdir/iZ > $rfile
	fi

# Form input file and run toporad

	mux $rfile $sfile $vfile $afile | toporad $net -r $R0 $diffuse
fi

# remove temporary files

rm -rf $tdir

exit 0
