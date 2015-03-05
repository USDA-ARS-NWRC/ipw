#-----------------------------------------------------------------------
# Copyright (c) 1990 The Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that: (1) source distributions retain this entire copyright
# notice and comment, and (2) distributions including binaries display
# the following acknowledgement:  ``This product includes software
# developed by the Computer Systems Laboratory, University of
# California, Santa Barbara and its contributors'' in the documentation
# or other materials provided with the distribution and in all
# advertising materials mentioning features or use of this software.
#
# Neither the name of the University nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#-----------------------------------------------------------------------

: ${IPW?}

## NAME
##	install -- install commands, libraries, etc.
##
## SYNOPSIS
##	install [-c] [-m mode] [-o owner] [-g group] [-s] file destination
##
## DESCRIPTION
##	install moves or copies (default: move) {file} to {destination},
##	optionally changing its owner, group, or permission mode.  If
##	{destination} is a directory, {file} will be placed in that
##	directory.
##
## OPTIONS
##	-c	{file} will be copied to {destination} rather than moved.
##
##	-m	The permission mode for the installed file will be set to
##		{mode}, which can be any mode recognized by chmod.
##
##	-o	The ownership of the installed file will be set to {owner}.
##		This may require root privileges (see the UNIX man page for
##		chown(1)).
##
##	-g	The group ownership of the installed file will be set to
##		{group}.  This may require either that you are a member of
##		the specified group, or root privileges (see the UNIX man
##		page for chgrp(1)).
##
##	-s	After installation, run the program strip to remove the
##		symbol tables.  This will make executables smaller, but
##		will remove any debugging information.  It only makes
##		sense for executable binary files.
##
## EXAMPLES
##	Within IPW, install is almost always invoked by make or ipwmake.
##
## FILES
##
## DIAGNOSTICS
##
## RESTRICTIONS
##
## FUTURE DIRECTIONS
##	install is currently implemented as a shell script.
##	It may be desirable to reimplement install as a program.
##
##	Perhaps GNU install, part of the fileutils package, should be used.
##
## HISTORY
##	Nov 90	Written by James Frew, UCSB.
##	Mar 97	Updated to use "getopts" instead of "getopt".  Modified
##		so this script is stand-alone (i.e., doesn't require
##		"$IPW/lib/ipwenv" script anymore).  J. Domingo, OSU
##	May 2009	If the destination doesn't exist, then ensure that
##			its parent directory exists.  J. Domingo, Green Code
##
## BUGS
##
## SEE ALSO
##	UNIX:  install, chmod, chgrp, chown, strip


pgm=`basename $0`
descrip='install commands, libraries, etc.'
synopsis='[-c] [-m mode] [-o owner] [-g group] [-s] file destination'
usage="Usage: $pgm $synopsis\n"

rm='rm -f'
create=mv

chmod=:
mode=
chown=:
owner=
chgrp=:
group=
strip=:

while getopts ':Hcg:m:o:s' opt ; do
	case $opt in
		H)	printf "\n$pgm -- $descrip\n\n"
			printf "$usage\n"
			exit 0
			;;
		c)	create=cp
			;;
		m)	chmod=chmod
			mode=$OPTARG
			;;
		o)	chown=chown
			owner=$OPTARG
			;;
		g)	chgrp=chgrp
			group=$OPTARG
			;;
		s)	strip=strip
			;;
		:)	echo OPTARG = '$OPTARG'
			;;
		\?)	shift `expr $OPTIND - 2`
			printf "\nERROR: bad option \"$1\"\n\n" >&2
			printf "$usage\n" >&2
			exit 1
			;;
	esac
done
shift `expr $OPTIND - 1`

case $# in
2)	src=$1
	dest=$2
	;;
*)	printf "$usage" >&2
	exit 1
	;;
esac

test -f $src || {
	printf "${pgm}: ERROR:\n" >&2
	printf "\tfile \"$src\": No such file\n" >&2
	exit 1
}

case $dest in
'.'|$src)
	create=:
	rm=:
	;;
*)	test -d $dest && dest=$dest/$src
	;;
esac

test -f $dest || {
    destDir=`dirname $dest`
    test -d $destDir || {
	mkdir -p $destDir
	printf "Created directory: $destDir\n"
    }
}

ls -l $src
$rm $dest
$create $src $dest
$strip $dest
$chmod $mode $dest
$chgrp $group $dest
$chown $owner $dest
ls -l $dest
