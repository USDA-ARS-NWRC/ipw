PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring="s:i:"
synopsis='[-s seed] [image ...]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $pgm "$synopsis" "$description"
}

seed=
image=

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	exec usage $pgm "$synopsis" "$description"
		;;
	-s)	seed="-s $2"
		shift
		;;
	*)	exec sherror $pgm "'getopt' failed"
		;;
	esac
	shift
done

trap 'rm -f $WORKDIR/*$$' 0
trap 'exit 0' 1 2 3 15

case $# in
0)	cat > $WORKDIR/rmize$$
	image=$WORKDIR/rmize$$
	;;
1)	image=$1
	case $image in
	'-')	cat > $WORKDIR/rmize$$
		image=$WORKDIR/rmize$$
		;;
	*)	test -r $image || {
			exec sherror $pgm "can't open file" $image
		}
		;;
	esac
	;;
*)	rm -f $WORKDIR/rmize$$
	for image in $*
	do
		case $image in
		-)	cat >> $WORKDIR/rmize$$
			;;
		*)	test -r $image || {
				exec sherror $pgm "can't open file" $image
			}
			cat $image >> $WORKDIR/rmize$$
			;;
		esac
	done
	image=$WORKDIR/rmize$$
	;;
esac


random -r 0,1000000 $seed -n `cat $image | wc -l` > $WORKDIR/rnd$$

paste $WORKDIR/rnd$$ $image | sort -n | sed 's/^[0-9]*[ 	]*//'

rm -f $WORKDIR/*$$

exit 0
