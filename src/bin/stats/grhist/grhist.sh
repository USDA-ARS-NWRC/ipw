PATH="$PATH:$IPW/lib"

synopsis='[graph-options]'
description='%description'

case $* in
-H)	exec usage $0 "$synopsis" "$description"
	;;
esac

graphPath=%GRAPH
if test -z "$graphPath"; then
	printf "Error: $0 uses the graph command, but that command can't be found.\n"
	exit 1
fi
if "$graphPath" --version < /dev/null > /dev/null 2>&1; then
	# GNU graph
	graphOpts=
else
	# Unix graph (e.g., Sun)
	graphOpts=-b
fi

if test -t 0; then
	exec usage $0  "$synopsis" "$description"
fi

primg -r -a |
	%AWK '$1 != 0 {
		print NR - 1, 0
		print NR - 1, $1
	}' |
	"$graphPath" $graphOpts $*

# Used to be quotes on the end of the second print statement:
#		print NR - 1, $1, "\" \""
# but that doesn't do anything with normal graph and breaks GNU graph
# which is necessary on DG/UX.

exit 0

