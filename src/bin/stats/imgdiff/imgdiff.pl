#!/usr/local/bin/perl -w
# By Rusty Dodson, 08/05/96.
#
# Report image difference stats.

use Getopt::Std;
getopts ('m:o:v');

if (@ARGV < 2 ) {
    $0 =~ s:.*/::;
    print <<EOF;

    $0 -- Compute statistics on the difference of two images

    Usage: $0 [-v] [-m mask] [-o diff_image] img1 img2
	img1	Input image (single-band)
	img2      "     "      "	
	-m	use mask image
	-o	output a difference image
	-v	verbose: compute mean absolute difference (MAD) 
		and root mean squared difference (RMSD).

EOF
exit 1;
}

$img1 = $ARGV[0];
$img2 = $ARGV[1];

(! -f $img1) && die "ERROR: $img1 not found.\n"; 
(! -f $img2) && die "ERROR: $img2 not found.\n"; 

if($opt_m) {
    (! -f $opt_m) && die "ERROR: $opt_m not found.\n"; 
    `ipwfile -mb $opt_m` > 1 and die "ERROR: mask image must be single-band.\n";
    $istat = "imgstat -m $opt_m";
    $lab   = "using mask $opt_m";
} else {
    $istat = "imgstat";
    $lab =   "";
}

if($opt_v) {
    open(IN, "mux $img1 $img2 | primg -a|") || 
	die "Error: can't open input pipe.\n";
    if($opt_m) {
        open(MASK, "primg -ari $opt_m|") || 
	    die "Error: can't open mask pipe.\n";
    }
    $sumsq = $sum = $npix = 0;
    while(<IN>) {
	($v1, $v2) = split(' ');
	if($opt_m) {
	    $m = <MASK>;
	    ($m == 0.0) && next;
	}
	$diff = $v1 - $v2;
	$sum += abs($diff);
	$sumsq += ($diff * $diff);
	$npix++;	
    }
    $mad = $sum / $npix;	
    $rmsd = sqrt($sumsq / $npix);
}

print "--- Stats on $img1 minus $img2 $lab ---\n";
system "mux $img1 $img2 | iadd -s 1 | $istat";
$? and die "ERROR: $0 failed.\n";

if($opt_v) {
    printf("%-15s %g  (npix = $npix) \n", "MAD:", $mad);
    printf("%-15s %g \n", "RMSD:", $rmsd);
}

if($opt_o) {
    print"\n\tmaking difference image $opt_o ...\n";
    system "mux $img1 $img2 | iadd -s 1 > $opt_o";
}
