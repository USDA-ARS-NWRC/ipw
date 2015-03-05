#  Convert integers in text format into 2-byte binary format

$packTemplate = 's';
$maxValue = 65_535;

while (<STDIN>) {
    @tokens = split;
    foreach my $token (@tokens) {
	if ($token !~ /^\d+$/) {
	    inputError("\"$token\" is not an integer");
        }
	$intValue = +$token;
	if ($intValue > $maxValue) {
	    inputError("$intValue > maximum value (= $maxValue)");
	}
	print pack($packTemplate, $intValue);
    }
}

exit 0;

#----------------------------------------------------------------------------

sub inputError {
    my $message = shift(@_);
    print STDERR "Error on line $.: $message\n";
    exit 1;
}
