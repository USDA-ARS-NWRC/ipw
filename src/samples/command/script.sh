#  This is a sample skeleton for a sh script to implement an IPW command.

#  The "synopsis" variable specifies the options and operands
synopsis='[-x] [-m mask] elev_image'

#  The description variable is set using an IPW macro
description="%description"
 
#  Add directory where shell-support commands reside to path
PATH="$PATH:$IPW/lib"

#  (Perform any other initialization)
count=7
mask=

#  Now, process options using "getopts".  Refer to its man page for
#  details about the syntax of the option string.
while getopts "Hxm:" opt
    do
    case $opt in
	-H)	#  Standard convention for IPW commands: -H to display usage
		exec usage $0 "$synopsis" "$description"
		;;
	-x)	count=100
		;;
	-m)	mask=$OPTARG
		;;
	*)	exec usage $0 "$synopsis" "$description"
		;;
    esac
    done

#  Now, fetch operands (if any)
case $# in
    0)	#  No operands, so use standard input by default, but first make sure
	#  it's not a terminal
	test -t 0 && exec sherror $0 "can't read image data from terminal"
	;;
    1)	image=$1
	;;
    *)	exec usage $0 "$synopsis" "$description"
	;;
esac

#  Do any other setup before processing operands
test -t 1 && exec sherror $0 "can't write image data to terminal"

#  Okay, now crunch through the operands
#  (e.g., do something with $image using $count and $mask)

#  Always exit with a successful status
exit 0
