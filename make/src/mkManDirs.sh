#  This shell script makes the directories where the man pages for library
#  functions reside.  The script must be invoked in a directory that represents
#  a category of library functions (see the include file "../funcCategory"). 
#
#  SYNOPSIS
#
#	script -d manDir [ function ] ...
#
#  The script creates a directory for each given function: {manDir}/{function}.
#  If the directory {manDir} does not exist, it will be created first.
#  In the directory for each function, the script creates initial copies
#  of these files:
#
#	Makefile
#	{function}.man
#
#  The intent is that the user will then edit the Makefile and place the
#  function's description in there.  The user can then edit the sample
#  source file for the function's man page: {function}.man.

script=`basename $0`
usage="Usage: $script -d manDir [ function ] ..."

if [ $# -lt 2 -o "$1" != '-d' ] ; then
    printf "$usage\n"
    exit 1
fi

manDir=$2
shift 2

#  Make sure the {manDir} directory exists

if ls -ld $manDir > /dev/null 2>&1 ; then
    if [ -d $manDir ] ; then
	printf "$manDir: directory exists\n"
    else
	printf "$manDir: already exists, but not a directory\n"
	exit 1
    fi
else
    printf "mkdir $manDir\n"
    mkdir $manDir
fi

#  For each function, make sure its associated directory exists in {manDir},
#   and create copies of its Makefile and man page if needed.

for func in $* ; do
    dir=$manDir/$func
    if ls -ld $dir > /dev/null 2>&1 ; then
	if [ -d $dir ] ; then
	    printf "$dir: directory exists\n"
	else
	    printf "$dir: already exists, but not a directory\n"
	    exit 1
	fi
    else
	printf "mkdir $dir\n"
	mkdir $dir
    fi

    #  Check if Makefile exists

    file=$dir/Makefile

    if ls -ld $file > /dev/null 2>&1 ; then
	if [ -f $file ] ; then
	    printf "$file: file exists\n"
	else
	    printf "$file: already exists, but not a file\n"
	    exit 1
	fi
    else
	cat > $file <<END
NAME := $func
DESCRIPTION := ($func's description)

include \$(IPW)/make/funcManpage
END
	printf "$file: file created\n"
    fi

    #  Check if man page's source file exists; if not, then make a sample

    manFile=$dir/$func.man

    if ls -ld $manFile > /dev/null 2>&1 ; then
	if [ -f $manFile ] ; then
	    printf "$manFile: file exists\n"
	else
	    printf "$manFile: already exists, but not a file\n"
	    exit 1
	fi
    else
	make -C $dir sample-man
    fi
done

exit 0
