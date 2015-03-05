PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='n:z:t:w:g:x:s:d:f:y:b:l:a:m:c:'
synopsis='[-n #bits[,#bits]] -z elev -t tau -w omega -g gfact -x w1,w2 \
	-s start -d day -f zone -y year	-b d,m,s -l d,m,s \
	-a gsize -m maxgsz -c dirt [image]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	usage $pgm "$synopsis" "$description"
	exit 1
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
nbits=
gsize=
maxgsz=
dirt=
start=
wyd=
wy=
zone=

# parse options, if none print description

case $# in
0|1)	usage $pgm "$synopsis" "$description"
	exit 1
	;;
esac

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	usage $pgm "$synopsis" "$description"
		exit 1
		;;
	-n)	nbits="-n $2"
		shift
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
	-a)	gsize=$2
		shift
		;;
	-m)	maxgsz=$2
		shift
		;;
	-c)	dirt=$2
		shift
		;;
	*)	sherror $pgm '"getopt" failed'
		exit 1
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
#
# alb - albedo image (1 band)
# shade - cosine illumination angle image, corrected for horizons (1 band)
# hor - horizon mask (1 band)

# Can only have 1 input image.  If stdin must make duplicate copy
# because we need multiple access.

img=$1
case $# in
0)	test -t 0 && {
	sherror $pgm "can't read image data from terminal"
	exit 1
	}
	;;
1)	;;
*)	exec usage $pgm "$synopsis" "$description"
	;;
esac


# make sure all essential arguments in

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

case $lat in
'')	exec sherror $pgm "-b arg missing or incomplete"
	;;
esac

case $lon in
'')	exec sherror $pgm "-l arg missing or incomplete"
	;;
esac



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
	ir_min=.28
	ir_max=.7
	v_min=.7
	v_max=2.8
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

# horizon output file
hfile=$tdir/hor

# shade output file
sfile=$tdir/shade

# salbedo output file
afile=$tdir/alb


#   Get sun angles for this time

angout=`sunang -t $date -b $lat -l $lon`
mu=`expr "$angout" : '.*-u \(.*\) -a'`
azm=`expr "$angout" : '.*-a \(.*\)'`
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

# Run salbedo to get albedo

	salbedo -s $start -d $wyd -g $gsize -m $maxgsz -c $dirt $wflag $sfile > $afile

# Run imgstat to get R0: mean albedo

	R0=`imgstat $afile | awk '/mean/ {print $2}'`

# Run elevrad to get beam & diffuse

	elevrad -n 8 -z $elev -t $tau -w $omega -g $gfact -r $R0 -s $S0 \
		-u $mu $tdir/iZ
fi

# remove temporary files

rm -rf $tdir

exit 0
