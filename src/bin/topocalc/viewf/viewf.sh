PATH="$PATH:$IPW/lib"

pgm=`basename $0`
synopsis='[-x] elev_image'
description='%description'

tdir=$TMPDIR/$pgm.$$

angles=16

while getopts 'Hx' c
do
	case $c in
	-)	shift
		break;;
	x)	angles=32;;
	H)	usage $pgm "$synopsis" "$description"
		exit 1;;
	*)	usage $pgm "$synopsis" "$description"
		exit 1;;
	esac
done
shift `expr $OPTIND - 1`

# must have exactly 1 input image

image=$1
case $# in
0)	sherror $pgm "can't read from standard input"
	usage $pgm "$synopsis" "$description"
	exit 1
	;;
1)	;;
*)	usage $pgm "$synopsis" "$description"
	exit 1
	;;
esac

if [ ! -r $image ] ; then
	sherror $pgm "can't open file", $image
	exit 1
fi

# can't write image data to terminal

test -t 1 && {
	sherror $pgm "can't write image data to a terminal"
	exit 1
}

trap 'rm -f -r $tdir' 0
trap 'exit 0' 1 2 3 15

# make directory for temporary files
mkdir $tdir

hor1d=$IPW/aux/horizon/hor1d

# horizons in E and W directions
$hor1d -a 90 $image > $tdir/horz.e
$hor1d -b -a 90 $image > $tdir/horz.w

# horizons in SSW and NNE directions
skew -a -22.5 $image | \
	transpose > $tdir/sxt1
$hor1d -a -22.5 $tdir/sxt1 | \
	transpose | \
	skew > $tdir/horz.ssw
$hor1d -b -a -22.5 $tdir/sxt1 | \
	transpose | skew > $tdir/horz.nne

# horizons in SW and NE directions
skew -a -45 $image | \
	transpose > $tdir/sxt2
$hor1d -a -45 $tdir/sxt2 | \
	transpose | \
	skew > $tdir/horz.sw
$hor1d -b -a -45 $tdir/sxt2 | \
	transpose | \
	skew > $tdir/horz.ne

# horizons in SSE and NNW directions
skew -a 22.5 $image | \
	transpose > $tdir/sxt3
$hor1d -a 22.5 $tdir/sxt3 | \
	transpose | \
	skew > $tdir/horz.sse
$hor1d -b -a 22.5 $tdir/sxt3 | \
	transpose | \
	skew > $tdir/horz.nnw

# horizons in SE and NW directions
skew -a 45 $image | \
	transpose > $tdir/sxt4
$hor1d -a 45 $tdir/sxt4 | \
	transpose | \
	skew > $tdir/horz.se
$hor1d -b -a 45 $tdir/sxt4 | \
	transpose | \
	skew > $tdir/horz.nw

# horizons in S and N directions
transpose $image > $tdir/xt
$hor1d -a 0 $tdir/xt | \
	transpose > $tdir/horz.s
$hor1d -b -a 0 $tdir/xt | \
	transpose > $tdir/horz.n

# horizons in ENE and WSW directions
skew -a -22.5 $tdir/xt | \
	transpose > $tdir/xts1
$hor1d -a 112.5 $tdir/xts1 | \
	transpose | \
	skew | \
	transpose > $tdir/horz.ene
$hor1d -b -a 112.5 $tdir/xts1 | \
	transpose | \
	skew | \
	transpose > $tdir/horz.wsw

# horizons in ESE and WNW directions
skew -a 22.5 $tdir/xt | \
	transpose > $tdir/xts2
$hor1d -a 67.5 $tdir/xts2 | \
	transpose | \
	skew | \
	transpose > $tdir/horz.ese
$hor1d -b -a 67.5 $tdir/xts2 | \
	transpose | \
	skew | \
	transpose > $tdir/horz.wnw


if [ $angles -eq 32 ] ; then
skew -a -11.25 $image | transpose > $tdir/sxt1s
$hor1d    -a -11.25 $tdir/sxt1s | transpose | skew > $tdir/horz.ssws
$hor1d -b -a -11.25 $tdir/sxt1s | transpose | skew > $tdir/horz.nnes

skew -a -33.75 $image | transpose > $tdir/sxt2s
$hor1d    -a -33.75 $tdir/sxt1s | transpose | skew > $tdir/horz.sws
$hor1d -b -a -33.75 $tdir/sxt1s | transpose | skew > $tdir/horz.nes

skew -a  11.25 $image | transpose > $tdir/sxt3s
$hor1d    -a  11.25 $tdir/sxt3s | transpose | skew > $tdir/horz.sses
$hor1d -b -a  11.25 $tdir/sxt3s | transpose | skew > $tdir/horz.nnws

skew -a  33.75 $image | transpose > $tdir/sxt4s
$hor1d    -a  33.75 $tdir/sxt4s | transpose | skew > $tdir/horz.ses
$hor1d -b -a  33.75 $tdir/sxt4s | transpose | skew > $tdir/horz.nws

skew -a -11.25 $tdir/xt | transpose >$tdir/xts1s
$hor1d    -a  101.25 $tdir/xts1s | transpose | skew | transpose > $tdir/horz.enes
$hor1d -b -a  101.25 $tdir/xts1s | transpose | skew | transpose > $tdir/horz.wsws

skew -a -33.75 $tdir/xt | transpose >$tdir/xts3s
$hor1d    -a  123.75 $tdir/xts3s | transpose | skew | transpose > $tdir/horz.enen
$hor1d -b -a  123.75 $tdir/xts3s | transpose | skew | transpose > $tdir/horz.wswn

skew -a  11.25 $tdir/xt | transpose >$tdir/xts2s
$hor1d    -a  78.75 $tdir/xts2s | transpose | skew | transpose > $tdir/horz.esen
$hor1d -b -a  78.75 $tdir/xts2s | transpose | skew | transpose > $tdir/horz.wnwn

skew -a  33.75 $tdir/xt | transpose >$tdir/xts4s
$hor1d    -a  56.25 $tdir/xts4s | transpose | skew | transpose > $tdir/horz.eses
$hor1d -b -a  56.25 $tdir/xts4s | transpose | skew | transpose > $tdir/horz.wnws

fi

# create gradient file
gradient $image > $tdir/gradient

# mux horizon together and pipe into view factor calculations
mux $tdir/horz.* | \
	%auxDir/viewcalc -s $tdir/gradient


exit 0
