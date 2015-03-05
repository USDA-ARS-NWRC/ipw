PATH="$PATH:$IPW/lib"

synopsis='[-n size] [-o min,max] [-i min,max] [-f floor] [-c ceil]'
description='%description'

# get command-line arguments

set -- `getopt 'n:o:i:f:c:' $* 2>/dev/null` ||
	exec usage $0 "$synopsis" "$description"

# avoid binary I/O on tty

test -t 0 && exec sherror $0 "can't read histogram from a terminal"
test -t 1 && exec sherror $0 "can't write lut to a terminal"

# parse options

size=256
o_args=
i_args=
floor=
ceil=

while :; do
	case $1 in
	--)	shift
		break
		;;
	-n)	size=$2
		shift
		;;
	-o)	o_args=$2
		shift
		;;
	-i)	i_args=$2
		shift
		;;
	-f)	floor=$2
		shift
		;;
	-c)	ceil=$2
		shift
		;;
	*)	exec shbug $0 "'getopt' failed"
		;;
	esac
	shift
done

# [[ There should be some code here to ensure that size is a power of 2. ]]

case $o_args in
'')	out_lo=0
	out_hi=`expr $size - 1`
	;;
*)	out_lo=`echo $o_args | awk -F, '{print $1}'`
	out_hi=`echo $o_args | awk -F, '{print $2}'`
	;;
esac

case $i_args in
'')	in_lo=0
	in_hi=`expr $size - 1`
	;;
*)	in_lo=$1`echo $i_args | awk -F, '{print $1}'`
	in_hi=$2`echo $i_args | awk -F, '{print $2}'`
	;;
esac

case $floor in
'')	floor=$out_lo
	;;
esac

case $ceil in
'')	ceil=$out_hi
	;;
esac

# derived parameters

nbits=`echo $ceil |
	%AWK '{
		nbits = 0;
		
		for (ceil = $1; ceil >= 1; ceil /= 2) {
			++nbits;
		}

		print nbits
	}'`

case $nbits in
0)	exec sherror $0 "bad option causes 0-bit output values"
	;;
esac

nbytes=`echo $nbits |
	%AWK '{
		nbytes = 1

		for (nbits = 8; $1 > nbits; nbits *= 2) {
			nbytes *= 2
		}

		print nbytes
	}'`

case $nbytes in
1|2|4)	;;
*)	exec sherror $0 "bad option causes ${nbytes}-byte output values"
	;;
esac

# write lookup table header to stdout

mkbih -l 1 -s $size  -i $nbits -f

# { equalization parameters;
#   normalized cumulative histogram < IPW histogram on stdin } |
# awk script generates LUT |
# convert to binary

{
	echo $in_lo $in_hi $out_lo $out_hi $floor $ceil
	cnhist $in_lo $in_hi
} | 
	%AWK '
		NR == 1 {
			in_lo = $1
			in_hi = $2
			out_lo = $3
			out_hi = $4
			floor = $5
			ceil = $6

			scale = out_hi - out_lo
			next
		}

		{ in_val = NR - 2 }

		in_val < in_lo {
			print floor
			next
		}

		in_val <= in_hi {
			print int(scale * $1 + out_lo) 
			next
		}

		in_val > in_hi {
			print ceil
		}
	' | 
	text2bin -$nbytes

exit 0

