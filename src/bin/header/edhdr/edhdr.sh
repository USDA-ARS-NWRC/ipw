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

PATH=$PATH:$IPW/lib

synopsis='image ...'
description='%description'

case $* in
''|-H)	usage $0 "$synopsis" "$description"
	exit 1
	;;
esac

test -t 0 -a -t 1 || {
	sherror $0 'standard input and output must be a terminal'
	exit 1
}

tmp=`basename $0`$$
trap 'rm -f $tmp' 0
trap 'exit' 1 2 3 15

for img do
	test -w $img || {
		sherror $0 'image not writable' $img
		exit
	}

	set -e

	echo "copying $img to $img.BAK ..."
	cp $img $img.BAK

	echo "extracting header into scratch file $tmp ..."
	prhdr $img >$tmp

	echo "editing $tmp ..."
	${EDITOR-vi} $tmp

	echo "updating $img:"

	echo "	copying new header into $img ..."
	# NB: replacement string is a control-L, NOT the 2 characters "^L"
	sed '$s/$//' $tmp >$img

	echo "	copying image data from $img.BAK into $img ..."
	rmhdr $img.BAK >>$img

	echo "	removing $img.BAK ..."
	rm $img.BAK

	set --
done
exit 0
