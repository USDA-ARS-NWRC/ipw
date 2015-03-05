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

	NR == 1 {
		xold = $1
		yold = $2
	}

	{
		xnew = $1
		ynew = $2
	}

	xnew != xold {
		dx = xnew - xold
		dydx = (ynew - yold) / dx

		if (dx < 0) {
			dx = -1
		}
		else {
			dx = 1
		}

		y = yold
		for (x = xold; x != xnew; x += dx) {
			print x, int(y + 0.5)
			y += dydx
		}

		xold = xnew
		yold = ynew
	}

	END {
		print x, int(yold + 0.5)
	}
'

case $? in
0)	exit 0
	;;
*)	exec sherror $0 'input line must have exactly 2 fields'
	;;
esac
