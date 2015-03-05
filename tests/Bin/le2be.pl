#  A Perl script to convert from little-endian to big-endian

$debugging = 0;

while (@ARGV) {
  $_ = shift @ARGV;
  /^-d/ && do { $debugging = 1; next; };
  push (@files, $_);
}

foreach $infile (@files) {
  &conv($infile);
}

exit 0;

sub conv {
  local ($file) = @_;

  open (INFILE, $file) || ((warn "Can't open $file: $!\n"), return 0);

  $header = <INFILE>;
  if ( $header !~ /^!<header>/ ) {
    #warn "$file is not an IPW file\n";
    return 0;
  }

  $order = <INFILE>;
  if ($order =~ /^byteorder = 3210/) {
    $fliporder = 0;
  } elsif ($order =~ /^byteorder = 0123/) {
    $fliporder = 1;
  } else {
    warn "$file does not have a byteorder header\n";
    close (INFILE);
    return 0;
  }

    open (OUTFILE, ">$file.be") || ((warn "Can't open output file $!\n"),
                                   return 0);
    print OUTFILE $header;
    print OUTFILE "byteorder = 3210 \n";
    $band = 0;
    while (<INFILE>) {
      print OUTFILE;
      last if /$/;
      last if eof(INFILE);
      if (/^nbands = (\d+)/) {
        $nbands = $1;
      }
      if (/^bytes = (\d+)/) {
        $bytes[$band++] = $1;
      }
    }
    if ($band != $nbands) {
      warn "Image has $nbands bands, but only $band BI headers.\n";
    }
    $debugging && print "ok, bands = ", $band, "\n";
    $useone = 1;
    for ($nb = 0; $nb < $band; $nb++) {
      $debugging && print "band $band has ", $bytes[$nb], " bytes\n";
      if ($bytes[$nb] > 1) {
        $useone = 0;
      }
      $tbytes += $bytes[$nb];
    }
    $debugging && print "useone is $useone\n";
    if ( ($useone == 1) || ($fliporder == 0) ) {
      $debugging && print "copying....\n";
      # just copy the data -- no multibyte bands exist
      while (read(INFILE, $buf, 16384)) {
        print OUTFILE $buf;
      }
    } else {
      # read a band worth and flip.  Very inefficient.
      while (read(INFILE, $buf, $tbytes)) {
        $pointer = 0;
        for ($nb = 0; $nb < $nbands; $nb++) {
          if ($bytes[$nb] == 1) {
            $pointer++;
          } elsif ($bytes[$nb] == 2) {
            substr($buf, $pointer, 2) =
                       substr($buf, $pointer+1, 1) . substr($buf, $pointer, 1);
            $pointer += 2;
          } elsif ($bytes[$nb] == 3) {
            # I don't think this is right...
            substr($buf, $pointer, 2) =
                       substr($buf, $pointer+1, 1) . substr($buf, $pointer, 1);
            $pointer += 3;
          } elsif ($bytes[$nb] == 4) {
            substr($buf, $pointer, 4) =   substr($buf, $pointer+3, 1)
                                        . substr($buf, $pointer+2, 1)
                                        . substr($buf, $pointer+1, 1)
                                        . substr($buf, $pointer, 1);
            $pointer += 4;
          } else {
            die "Can't deal with ", $bytes[$nb], " byte bands.\n";
          }
        }
        print OUTFILE $buf;
      }
    }

  close (INFILE);
  close (OUTFILE);
  rename("$file.be", "$file");
  return 1;
}
