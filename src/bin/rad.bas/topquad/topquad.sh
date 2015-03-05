PATH="$PATH:$IPW/lib"

pgm=`basename $0`
optstring='nz:t:w:g:r:s:x:d:b:l:'
synopsis='[-n] -z elev -t tau -w omega -g g -r R0 [-s S0] [-x w1,w2] \
	-d y,m,d -b d,m,s -l d,m,s [image]'
description='%description'

set -- `getopt "$optstring" $* 2>/dev/null` || {
	exec usage $pgm "$synopsis" "$description"
}

elev=
tau=
omega=
gfact=
R0=
S0=
wrange=
year=
month=
day=
date=
lat=
lon=
net=0

# parse options, if none print description

case $# in
0|1)	exec usage $pgm "$synopsis" "$description"
	;;
esac

while :; do
	case $1 in
	--)	shift
		break
		;;
	-H)	exec usage $pgm "$synopsis" "$description"
		;;
	-n)	net=1
		;;
	-z)	elev=$2
		shift
		;;
	-t)	tau=$2
		shift
		;;
	-w)	omega=$2
		shift
		;;
	-g)	gfact=$2
		shift
		;;
	-r)	R0=$2
		shift
		;;
	-s)	S0=$2
		shift
		;;
	-x)	wrange=$2
		shift
		;;
	-d)	date=$2
		shift
		;;
	-b)	lat=$2
		shift
		;;
	-l)	lon=$2
		shift
		;;
	*)	exec sherror $pgm '"getopt" failed'
		;;
	esac
	shift
done

tdir=$TMPDIR/$pgm.$$
mkdir $tdir

# list of temporary files used (i.. image; a.. ascii)
# directory for all is $TMPDIR/$pgm.$$
#
# iX - copy of input if stdin
# iZ - elevation image (1 band)
# iGR - gradient image (2 bands)
# iVF - view factor/albedo image (3 bands)
# aQ - quadrature times (sunlight output)
# aS - input for final awk script
# sh - sequence of commands to run
#
# the rest of the files are named within awk script
#
# rad.xx - interval radiation files to be summed by lincom
#
# the following files are re-created for each time step
#
# shade - cosine illumination angle, corrected for horizons
# hor - horizon mask
# erad - beam and diffuse radiation over elevation grid

# Can only have 1 input image.  If stdin must make duplicate copy
# because we need multiple access.

img=$1
case $# in
0)	test -t 0 && {
	exec sherror $pgm "can't read image data from terminal"
	}
	;;
1)	;;
*)	exec usage $pgm "$synopsis" "$description"
	;;
esac

case $img in
''|'-')
	cat $img > $tdir/iX
	image=$tdir/iX
	;;
*)	test -r $img || {
		exec sherror $pgm "can't open file", $image
	}
	image=$img
	;;
esac

# can't write image data to terminal

test -t 1 && {
	exec sherror $pgm "can't write image data to a terminal"
}

# parse date into year, month, day

case $date in
'')	exec usage $pgm "$synopsis" "$description"
	;;
*)	year=` echo $date | awk -F, '{print $1}'`
	month=`echo $date | awk -F, '{print $2}'`
	day=`  echo $date | awk -F, '{print $3}'`
	;;
esac

# make sure all essential arguments in

case $elev in
'')	exec sherror $pgm "-z arg missing"
	;;
esac

case $tau in
'')	exec sherror $pgm "-t arg missing"
	;;
esac

case $omega in
'')	exec sherror $pgm "-w arg missing"
	;;
esac

case $gfact in
'')	exec sherror $pgm "-g arg missing"
	;;
esac

case $R0 in
'')	exec sherror $pgm "-r arg missing"
	;;
esac

case $S0 in
'')	case $wrange in
	'')	exec sherror $pgm "-s and -x args missing, one must be present"
		;;
	*)	S0=`solar -d $date -w $wrange -a`
		;;
	esac
esac

case $year in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $month in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $day in
'')	exec sherror $pgm "-d arg missing or incomplete"
	;;
esac

case $lat in
'')	exec sherror $pgm "-b arg missing or incomplete"
	;;
esac

case $lon in
'')	exec sherror $pgm "-l arg missing or incomplete"
	;;
esac


# elevation file
demux -b 0 $image > $tdir/iZ

# slope/aspect file
demux -b 1,2 $image > $tdir/iGR

# sky view, terrain view, and albedo
demux -b 3,4,5 $image > $tdir/iVF

# 1st awk script writes weights and solar angles to temporary file

echo $date $lat $lon | %AWK '
	{
		print "sunlight -d", $1, "-b", $2, "-l", $3, "-q 21 -a"
	}' | sh > $tdir/aQ

# 2nd awk script uses these values to create processing script for elevrad
# (all args except -u are constant)

echo $net $elev $tau $omega $gfact $R0 $S0 \
	$tdir/iZ $tdir/iGR $tdir/iVF $tdir/ \
	> $tdir/aS

cat $tdir/aS $tdir/aQ | %AWK '
BEGIN {
	already = 0
	j = 0
}
{
	if (already) {
		wt[j] = $1
		mu = $2
		azm = $3
		rfile = sprintf("%serad", froot);
		print ecmd, mu, "\\"
		print "	", efile, ">", rfile
		hfile = sprintf("%shor", froot);
		print "horizon -u", mu, "-a", azm, "\\"
		print "	", efile, ">", hfile
		sfile = sprintf("%sshade", froot);
		print "shade -u", mu, "-a", azm, grfile, "|"
		print "	mux", "-", hfile, "|"
		print "	bitcom -m -a >",sfile
		tfile[j] = sprintf("%srad.%02d", froot, j);
		print "mux", rfile, "\\"
		print "	", sfile, "\\"
		print "	", vfile, "|"
		print "	", toporad, ">", tfile[j]
		++j
		print ""
	}
	else {
		already = 1
		if ($1 == 0)
			toporad = "toporad"
		else
			toporad = "toporad -n"
		elev = " -z " $2
		tau = " -t " $3
		omega = " -w " $4
		gfact = " -g " $5
		R0 = " -r " $6
		S0 = " -s " $7
		efile = $8
		grfile = $9
		vfile = $10
		froot = $11
		ecmd = "elevrad -n 8 " elev tau omega gfact R0 S0 " -u"
	}
}
END {
	n = j
	print "mux", "\\"
	for (j = 0; j < n; ++j)
		print "	", tfile[j], "\\"
	ORS = " "
	print "| lincom -c "
	ORS = ","
	for (j = 0; j < n-1; ++j)
		print wt[j]
	ORS = " "
	print wt[n-1]
}' > $tdir/sh$$

# run command string from temporary file

chmod +x $tdir/sh$$
sh -c $tdir/sh$$

# remove temporary files

rm -rf $tdir

exit 0

# $Header: /usr/home/dozier/ipw/src/bin/topquad/RCS/topquad.sh,v 1.5 89/07/06 22:02:15 dozier Exp $
