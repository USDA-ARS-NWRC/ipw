PATH="$PATH:$IPW/lib"

optstring=
synopsis=''
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $0 "$synopsis" "$description"
}

while :; do
	case $1 in
	--)	shift
		break
		;;
	*)	exec sherror $0 '"getopt" failed'
		;;
	esac

	shift
done

%AWK '
	BEGIN {
		stderr = "cat 1>&2"
	}

	NF != 2 {
		print "BAD INPUT:", $0 | stderr
		exit 1
	}
	{
		x[NR] = $1
		y[NR] = $2
		sum += $1
	}
	END {
		x_bar = sum / NR

		for (i = 1; i <= NR; i++) {
			sxy2 = sxy2 + (( x[i] - y[i] ) ^ 2 )
			sxxbar2 = sxxbar2 + (( x[i] - x_bar ) ^ 2 )
			syx2 = syx2 + (( y[i] - x[i] ) ^ 2 )
			syx = syx + (( y[i] - x[i] ))
		}
		me = 1.0 - ( sxy2 / sxxbar2 )
		rmsd = sqrt ( syx2 / NR )
		mbd = syx / NR
	
		print "n = ", NR
		print "mean = ", x_bar
		print "Model Efficiency, ME", me
		print "RMSD", rmsd
		print "Mean Bias Difference, MBD", mbd
	}
'

case $? in
0)	exit 0
	;;
*)	exec sherror $0 'input line must have exactly 2 fields'
	;;
esac
