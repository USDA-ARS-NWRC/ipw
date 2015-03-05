PATH="$PATH:$IPW/lib"

optstring='i:o:k:'
synopsis='[-i in_nbits] [-o out_nbits] [-k bkgd]'
description='%description'

# get command-line arguments

set -- `getopt "$optstring" $* 2>/dev/null` ||
	exec usage $0 "$synopsis" "$description"

if test -t 1; then
	sherror $0 "can't write lookup table to a terminal"
	exec usage $0 "$synopsis" "$description"
fi

ibits=8
obits=8
const=0

while :; do
	case $1 in
	--)	shift
		break
		;;
	-i)	ibits=$2
		isposint $ibits ||
			exec sherror $0 "$ibits: not a positive integer"
		shift
		;;
	-o)	obits=$2
		isposint $obits ||
			exec sherror $0 "$obits: not a positive integer"
		shift
		;;
	-k)	const=$2
		isposint $const ||
			exec sherror $0 "$const: not a positive integer"
		shift
		;;
	*)	exec sherror $pgm '"getopt" failed'
		;;
	esac
	shift
done
#DEBUG echo "ibits=$ibits"
#DEBUG echo "obits=$obits"
#DEBUG echo "const=$const"

obytes=`echo $obits |
        %AWK '{
                nbytes = 1

                for (nbits = 8; $1 > nbits; nbits *= 2) {
                        nbytes *= 2
                }

                print nbytes
        }'`

nsamps=`echo "2 ${ibits}^p" |
	dc`

#isposint $nsamps || exec sherror $0 "$ibits: too many bits per input pixel"
#DEBUG echo "nsamps=$nsamps"

{
	echo $nsamps $const
	cat
} |
	%AWK '
		NR == 1 {
			max = $1
			k = $2
			next
		}
		{
			for (; i < $1; ++i)
				print k

			print $2
			++i
		}
		END {
			for (; i < max; ++i)
				print k
		}
	' |
	text2bin -$obytes |
	mkbih -l 1 -s $nsamps  -i $obits 

exit $?
