# Synopsis
#
#   script [ -n ] [ -e ] [ -h ] [ -d ] cmdName output
#
#   cmdName = name of the command that is being tested
#   output  = name of image or text file to be compared against its counterpart
#             in the ../Outputs and ../Output-SP directories.
#
# Options
#
#   -n  indicates that this is not the last test.
#
#   -e  indicates that a test should be made for small floating point
#       differences betweent the files, which must be IPW images
#
#   -h  indicates that history headers should be striped from the
#       input image.
#
#   -d  indicates that the output file is a text file with columns of
#       numbers that should be compared to its counterpart, to see if
#	its values are all within specific deltas of their expected values

: ${IPW?}

TMPDIR=${TMPDIR:-$IPW/scratch}

dolitbig="true"
dotouch="true"
doediff=
doshist=
dodelta=

for arg in $*;
do
  case $arg in
  -n)
	dotouch=
	;;
  -e)
	doediff="true"
	;;
  -h)
	doshist="true"
	;;
  -d)
	dodelta="true"
	;;
  -*)
	printf "Invalid option \'$arg'\n" 1>&2
	exit 1
	;;
  *)
	if [ x$prog = x ]
	then
		prog=$arg
	fi
	file1=$arg
	;;
  esac
done

tmp0=$TMPDIR/`basename $0`$$-0
tmp1=$TMPDIR/`basename $0`$$-1
tmpf=$TMPDIR/`basename $0`$$-f

progfail="$prog-fail"
file2=Outputs/$file1
file2s=Outputs-SP/$file1

trap 'rm -f $tmp0 $tmp1 $tmpf $file1' 0
trap 'exit 0' 1 2 3 15

log=Test.log

progErrorDir=Errors/$prog
rm -f $progErrorDir/$file1
if [ -d $progErrorDir ] && [ `ls $progErrorDir | wc -l` = 0 ] ; then
   rmdir $progErrorDir
fi

if test "$doshist" = "true" ; then
	# strip history from file1 and replace it
	striphist $file1 >$tmpf
	rm -f $file1
	mv $tmpf $file1
fi

if test "$dolitbig" = "true" ; then
	# Convert file from little endian to big endian
	le2be $file1
fi

unset dofail
test -z "$dotouch" && dofail="true"
if [ -r "$progfail" ]
then
	test -z "$dofail" && rm -f $progfail
	unset dotouch
else
	rm -f $progfail
fi


if [ ! -s "$file1" ]
then
	test -n "$dofail" && touch $progfail
	printf "*** Command failed:      $prog\n"
	exit 0
fi

if [ ! -r "$file2" ]
then
	test -n "$dofail" && touch $progfail
	printf "*** Can't find outputs:  $prog\n"
	exit 0
fi

if cmp -s "$file1" "$file2" ; then
	test -n "$dotouch" && touch $prog
	test -n "$dotouch" && printf "    Command passed:      $prog\n"
	exit 0
fi

if [ -r "$file2s" ]
then
	if cmp -s "$file1" "$file2s" ; then
		test -n "$dotouch" && touch $prog
		test -n "$dotouch" && printf "    Command passed (sp): $prog\n"
		exit 0
	fi
fi

 

if test "$doediff" = "true" ; then
#   rmhdr $file1 > $tmp0
#   rmhdr $file2 > $tmp1
#   if test "`cmp $tmp0 $tmp1`" = "" ; then
#      rm -f $tmp0 $tmp1
#      test -n "$dotouch" && touch $prog
#      printf "  * Header difference:   $prog\n"
#      exit 0
#   fi
#   rm -f $tmp0 $tmp1
   if test "`ipwfile -mb $file1`" = "1  " ; then
      fdiff="`mux $file1 $file2 | lincom -c 1,-1 | imgstat | \
              awk '/mean/ {print $2}'`"
   else
      lincom $file1 > $tmp0
      lincom $file2 > $tmp1
      fdiff="`mux $tmp0 $tmp1 | lincom -c 1,-1 | imgstat | \
              awk '/mean/ {print $2}'`"
      rm -f $tmp0 $tmp1
   fi
   if test "`echo \"$fdiff 0.0001\" | awk '$1 > $2'`" = "" ; then
#     test -n "$dotouch" && touch $prog
     printf "  * Small float diff:    $prog\n"
   else
     test -n "$dofail" && touch $progfail
     printf " ** Different outputs:   $prog   (mean diff:$fdiff)\n"
   fi
   if [ ! -d $progErrorDir ] ; then
     mkdir -p $progErrorDir
   fi
   cp $file1 $progErrorDir/$file1
   exit 0
fi

#  Compare the actual file with expected file using deltas?
if [ -n "$dodelta" ] ; then
    printf "testdeltas $file2 $file1 ${file2}.deltas\n" >> $log
    if testdeltas $file2 $file1 ${file2}.deltas >> $log 2>&1 ; then
	printf "    Command passed:      $prog  (small float-pt differences)\n"
	exit 0
    fi
fi

test -n "$dofail" && touch $progfail
printf " ** Different outputs:   $prog\n"
if [ ! -d $progErrorDir ] ; then
   mkdir -p $progErrorDir
fi
cp $file1 $progErrorDir/$file1

exit 0
