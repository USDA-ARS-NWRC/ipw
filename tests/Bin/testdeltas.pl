#  SYNOPSIS
#	script expectedFile actualFile deltas
#
#  A Perl script that compares a text file against an expected text file.
#  The text file is the actual output file from a IPW command consisting
#  of a column of numbers.  The expected text file is the expected results
#  from the command.
#
#  This script checks that the actual file has the same number of lines
#  and columns as the expected file.  It also checks each value in the
#  actual file to its expected value; if the absolute difference between
#  them is greater than the corresponding delta in the third file, the
#  command prints a message noting the line and column of the value.
#
#  The format of the deltas file is one value per line.  The first value
#  is the delta for the first column of values in the expected file,
#  the second value is the delta for the second column, and so on.  In
#  the deltas file, a comment may follow the value on a line.  The
#  comment starts with the "#" character, and is ignored.
#
#  EXIT STATUS
#	0	if # of lines matches, # of columns matches, and all
#		values are within the threshold.
#
#	1	otherwise.

if ($#ARGV != 2) {
    print STDERR "Usage: $0 expectedFile actualFile deltas\n";
    exit 1;
}

$expectedFile = shift @ARGV;
if (! open(EXPECTED, "<$expectedFile")) {
    print STDERR "$0: cannot open file: $expectedFile\n";
    exit 1;
}

$actualFile = shift @ARGV;
if (! open(ACTUAL, "<$actualFile")) {
    print STDERR "$0: cannot open file: $actualFile\n";
    exit 1;
}

#  Load deltas

$deltaFile = shift @ARGV;
if (! open(DELTA, "<$deltaFile")) {
    print STDERR "$0: cannot open file: $deltaFile\n";
    exit 1;
}

while (<DELTA>) {
   ($delta) = split /\s+/, $_, 2;
   push @deltas, $delta;
}
$numColumns = $#deltas + 1;

$line = 0;
$count = 0;

while ($expectedLine = <EXPECTED>) {
    $line++;
    if ($actualLine = <ACTUAL>) {
	#  Check the actual values to the expected values
	@actuals = split '\s+', $actualLine;
	@expected = split '\s+', $expectedLine;
	$numActuals = $#actuals + 1;
	$numExpected = $#expected + 1;
	if ($numExpected != $numColumns) {
	    printf STDERR "$0: file does not have %d values on line %d: %s\n",
			  $numColumns, $line, $expectedFile;
	    exit 1;
	}
	if ($numActuals != $numExpected) {
	    printf STDERR "$0: line %d has %s values than expected: %s\n",
			 $line, ($numActuals < $numExpected ? "fewer" : "more"),
			 $actualFile;
	    exit 1;
	}

	$column = 0;
	foreach $value (@expected) {
	    $column++;
	    $actual = shift @actuals;
	    $delta = $deltas[$column-1];
	    $diff = $value - $actual;
	    if (abs($diff) > $delta) {
		$count++;
		if ($count == 1) {
		    print STDERR "Difference between expected & actual values",
				 " exceeds delta at\nthese locations:\n";
		}
		print STDERR "\tline $line, column $column (difference = $diff)\n";
	    }
	}
    } else {
	print STDERR "$0: file has fewer lines than expected: $actualFile\n";
	exit 1;
    }
}  # while

if ($actualLine = <ACTUAL>) {
    print STDERR "$0: file has more lines than expected: $actualFile\n";
    exit 1;
}

if ($count > 0) {
    exit 1;
}

exit 0;
