# striphist -- strip history lines from a file.

: ${IPW?}

PATH="$PATH:$IPW/lib"

tmp=$WORKDIR/`basename $0`$$-1
trap 'rm -f $tmp' 0
trap 'exit 0' 1 2 3 15

case $* in
'')	cat > $tmp
	file=$tmp
	break
	;;
*)	file=$*
esac

prhdr $file | %AWK '$0 !~ /^history = /' | %SED '$s/$//'
rmhdr $file

exit 0
