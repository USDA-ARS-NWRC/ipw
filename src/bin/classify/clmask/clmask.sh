pgm=`basename $0`

synopsis='-c class [-i image]'
description='%description'

# get command-line arguments

case $* in
''|-H)	exec usage $0 "$synopsis" "$description"
	;;
esac

optstring="c:i:"
set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $0 "$synopsis" "$description"
}

class=
image=

while :; do
	case $1 in
	--)	shift
		break
		;;
	-c)	class=$2
		shift
		;;
	-i)	image=$2
		shift
		;;
	*)	exec sherror $0 "'getopt' failed"
		;;
	esac
	shift
done

tmpimg=

case $image in
''|'-')
	trap 'rm -f $tmpimg' 0
	trap 'exit' 1 2 3 15
	tmpimg=$TMPDIR/$pgm.$$
	cat > $tmpimg
	image=$tmpimg
	;;
*)	test -r $image || {
		exec sherror $pgm "can't open file", $image
	}
	;;
esac

# avoid binary I/O on tty

if test -t 1; then
	exec sherror $0 "can't write image to a terminal"
fi

# find bytes/pixel of image

bytes=`ipwfile -my $image`

# copy header of classified image

prhdr $image | sed '$s/$//'

# write mask image

rmhdr $image | bin2text -$bytes | \
  %AWK '{if ($1 == '$class') print $1
       else print 0}' | \
  text2bin -$bytes

exit 0
