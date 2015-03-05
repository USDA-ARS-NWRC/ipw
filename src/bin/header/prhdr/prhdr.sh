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

PATH="${PATH}:${IPW}/lib"

synopsis='[file ...]'
description='%description'

case $1 in
'')	set -- -
	;;
-)	;;
-*)	exec usage $0 "$synopsis" "$description"
	;;
esac

for file do
	case $# in
	0|1)	;;
	*)	echo "::::::::::::::
$file
::::::::::::::"
		;;
	esac

	case $file in
	-)	file=
		;;
	*)	test -r $file || exec sherror $0 "can't open file" $file
		;;
	esac

	sed '
		// {
			s///
			q
		}
	' $file
done

exit 0
