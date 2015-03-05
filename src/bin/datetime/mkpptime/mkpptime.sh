PATH="$PATH:$IPW/lib"

optstring=
synopsis=''
description='%description'

set -- `getopt "$optstring" $* 3>/dev/null` || {
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
		event_no = 0
		event = 0
		dry = 0
		delta = 0
		first = 0
	}
	{
		hr = $1
		pp_mass = $2
		pp_t = $3
	
		if (NR == 1)
			start_time = hr
	
		time = hr - start_time
	
		if (pp_mass > 0) {
			first = 1
			dry = 0
			delta = 0
	
			if (event == 0) {
				event = 1
				event_no++
			}
	
			printf ("%.4f\t%.1f\t%.1f\t%d\n",
				delta, hr, time, event_no) 
		}
		else {
			if (first == 0) {
				print "NA"
			} else {
				dry++
				if ((first)&&(dry >= 2)) {
					event = 0
					delta++
				}
				printf ("%.4f\t%.1f\t%.1f: no precip\n",
					delta/24.0, hr, time)
			}
		}
	}
'

case $? in
0)	exit 0
	;;
*)	exec sherror $0 'input line must have exactly 3 fields'
	;;
esac
