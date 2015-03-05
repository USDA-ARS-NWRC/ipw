PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='a:'
synopsis='-a angle [image]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	usage $pgm "$synopsis" "$description"
	exit 1
}

angle=0

# parse options

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	exec usage $pgm "$synopsis" "$description"
		;;
	-a)	angle=$2
		shift
		;;
	*)	exec sherror $pgm '"getopt" failed'
		;;
	esac
	shift
done

# can only have 1 input image

image="-"
for image do
	case $# in
	0|1)	;;
	*)	exec usage $pgm "$synopsis" "$description"
		;;
	esac

	case $image in
	-)	;;
	*)	test -r $image || {
			exec sherror $pgm "can't open file" $image
		}
		;;
	esac
done

# can't write image data to terminal

test -t 1 && {
	exec sherror $pgm "can't write image data to a terminal"
}

pi=3.141592653589793
angleR=`printf "$angle $pi\n" | %AWK '{printf "%.14f", $1 * ($2/180)}'`
angled2=`printf "$angle\n" | %AWK '{printf "%.14f", $1 / 2}'`
angletsR=`printf "$angleR\n" | %AWK '{printf "%.14f", atan2(sin($1),1)}'`
anglets=`printf "$angletsR $pi\n" | %AWK '{printf "%.14f", $1 * (180/$2)}'`

skew -h -a $angled2 $image | flip -l | transpose | \
skew -h -a $anglets | flip -s | transpose | \
skew -h -a $angled2

exit 0
