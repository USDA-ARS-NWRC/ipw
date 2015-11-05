PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='a:d:z:u:'
synopsis='-a azimuth [-d delta] [-z zenith] [-u cos] [image]'
description='%description'
 
set -- `getopt "$optstring" $* 2>/dev/null` || {
	usage $pgm "$synopsis" "$description"
	exit 1
}

phi=
delta=0
zen=0
cosZ=0

# parse options

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	usage $pgm "$synopsis" "$description"
		exit 1
		;;
	-a)	phi=$2
		shift
		;;
	-d)	delta=$2
		shift
		;;
	-z)	zen=$2
		shift
		;;
	-u)	cosZ=$2
		shift
		;;
	*)	sherror $pgm '"getopt" failed'
		exit 1
		;;
	esac
	shift
done

case $phi in
'')	usage $pgm "$synopsis" "$description"
	exit 1
	;;
esac

# can only have 1 input image

image="-"
for image do
	case $# in
	0|1)	;;
	*)	usage $pgm "$synopsis" "$description"
		exit 1
		;;
	esac

	case $image in
	-)	;;
	*)	test -r $image || {
			sherror $pgm "can't open file", $image
			exit 1
		}
		;;
	esac
done

# can't write image data to terminal

test -t 1 && {
	sherror $pgm "can't write image data to a terminal"
	exit 1
}

# see if we can remove temp file if we get stopped early
trap 'rm -f $TMPDIR/$pgm$$' 0
trap 'exit 0' 1 2 3 15

# awk script creates output command string and puts in temporary file

echo $phi $delta $zen $cosZ $image | %AWK '
	{
		phi = $1
		delta = $2
		zen = $3
		cosZ = $4
		image = $5
		if (substr(image,1,1) == "#")
			image = sprintf("\\%s", $5);
		skew = "skew -a"
		unskew = "skew"
		if (delta == 0) {
			ds = ""
		}
		else {
			ds = sprintf(" -d %g", delta);
		}
		if (zen == 0) {
			dz = ""
		}
		else {
			dz = sprintf(" -z %g", zen);
		}
		if (cosZ == 0) {
			dc = ""
		}
		else {
			dc = sprintf(" -u %g", cosZ);
		}
		hf = "%auxDir/hor1d" ds dz dc " -a"
		hb = "%auxDir/hor1d -b" ds dz dc " -a"
		xp = "transpose"
		if (phi == 90)
			print hf, phi, image
		else if (phi == -90)
			print hb, -phi, image
		else if (phi == 0)
			print xp, image, "|", hf, phi, "|", xp
		else if (phi == -180)
			print xp, image, "|", hb, phi+180, "|", xp
		else if (phi == 180)
			print xp, image, "|", hb, phi-180, "|", xp
		else if (-45 <= phi && phi <= 45)
			print skew, phi, image, "|", xp, "|", hf, phi, "|", xp, "|", unskew
		else if (-135 >= phi && phi > -180)
			print skew, phi+180, image, "|", xp, "|", hb, phi+180, "|", xp, "|", unskew
		else if (180 > phi && phi >= 135)
			print skew, phi-180, image, "|", xp, "|", hb, phi-180, "|", xp, "|", unskew
		else if (45 < phi && phi < 135)
			print xp, image, "|", skew, 90-phi, "|", xp, "|", hf, phi, "|", xp, "|", unskew, "|", xp
		else if (-45 > phi && phi > -135)
			print xp, image, "|", skew, -90-phi, "|", xp, "|", hb, phi+180, "|", xp, "|", unskew, "|", xp
		else
			print "error: phi =", phi
	}' > $TMPDIR/$pgm$$

# run command string from temporary file

chmod +x $TMPDIR/$pgm$$
sh -c $TMPDIR/$pgm$$

# remove temporary file

rm -f $TMPDIR/$pgm$$

exit 0
