# grid2ipw - Convert an image created by arc/info into an ipw image.
#	This script is usually called by GRID2IPW.AML.
#       "prefix.bip" has image data, "prefix.info" has parameters.
#
# By Rusty Dodson, 04/25/96.

if (@ARGV < 2) {
    $0 =~ s:.*/::;
    print <<EOF;

    $0 -- Convert an image created by Arc/Info into an IPW image. 
	  	(Called by GRID2IPW.AML)

    Usage: $0 in_prefix out_image

	in_prefix - Input file prefix.  Expected files: .bip and .info
	out_image - Output IPW image.

EOF
exit 1;
}

$prefix = $ARGV[0];
$outimg = $ARGV[1];

### unknown fields may be dealt with later

$geou = "unknown";  	# units of geodetic info
$csys = "unknown";	# coordinate system
$lqu  = "unknown";	# units of lq pixels 

# annotation 
chomp($date = `date '+%m/%d/%y'`);
$annot = "Transferred from Arc/Info on " . $date;

### check on image data

(! -e "$prefix.bip") && die "ERROR: $0:\n\t can't find $prefix.bip\n";

### read stuff from the .info file, delimited by whitespace

open(INFO, "$prefix.info") || die "ERROR: $0:\n\t can't open $prefix.info\n";

while(<INFO>) {
    ($key, $val) = split(' ');
    if($key eq "lines")  { $lines = $val; }
    if($key eq "samps")  { $samps = $val; }
    if($key eq "bits")   { $bits  = $val; }
    if($key eq "bands")  { $bands = $val; }
    if($key eq "bline")  { $bline = $val; }
    if($key eq "bsamp")  { $bsamp = $val; }
    if($key eq "dline")  { $dline = $val; }
    if($key eq "dsamp")  { $dsamp = $val; }
    if($key eq "fmin")   { $fmin  = $val; }
    if($key eq "fmax")   { $fmax  = $val; }
    if($key eq "imin")   { $imin  = $val; }
    if($key eq "imax")   { $imax  = $val; }
}

### make headers and attach to image data

system "mkbih -l $lines -s $samps -b $bands -i $bits -a \"$annot\" " .
	    " $prefix.bip " .
       " | mkgeoh -o $bline,$bsamp -d $dline,$dsamp -u $geou -c $csys" .
       " | mklqh -m $imin,$fmin,$imax,$fmax -u $lqu" .
       " > $outimg";


foreach $suff (".bip", ".info", ".hdr", ".stx", ".bpw") {
    unlink("$prefix.{$suff}");
}
