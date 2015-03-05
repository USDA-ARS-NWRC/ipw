#!/usr/local/bin/perl
#
# implementation of IPW grhist in perl
#
$GRAPH = "/usr/local/gnu/bin/gnugraph";

undef $/;                    # make perl slurp up entire file with one read

$histogram = <>;             # get IPW histogram
$hlen = length($histogram);  # end of histogram

$offset = 0;
$image = index($histogram, pack("c", 12)) + 2;  # start of image after ^L

for ($i = $image; $i < $hlen; $i += 4) {
   $value = unpack("l", substr ($histogram, $i, 4));
   $bin[$offset++] = $value;
   $total += $value;
}

open(GRAPF, "| $GRAPH");

for ($i = 0; $i < $offset; $i++) {
  $frac = $bin[$i] / $total;
  print GRAPF "$i  $frac \n";
}

close GRAPF;

