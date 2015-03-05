#  A perl script to convert a text file with the list of constant names
#  into two files:
#
#  1) a header file defining the constants as integers:
#	the first constant is defined as 0,
#	the second constant is defined as 1,
#	and so on
#
#  2) a C source code file with the constants as string literals:
#	e.g., "APPLE",
#	      "BANANA",
#	      "CAKE"
#     this segment of C source code is meant to initialize an array
#     indexed by the constants in the header file (for printing)
#
#  The script requires two arguments:
#
#	first argument = name of header file
#	second argument = name of C source code file

if ($#ARGV != 1) {
	print STDERR "Usage: $0 header-file source-code-file\n";
	exit 1;
}

$headerFile = shift @ARGV;
$sourceFile = shift @ARGV;

if ($headerFile eq $sourceFile) {
	print STDERR "header-file and source-code-file are the same file\n";
	exit 1;
}

open HDR_FILE, ">$headerFile" or
	die "Can't open header file";
open SRC_FILE, ">$sourceFile" or
	die "Can't open source code file";

@names = ();
$maxLen = 0;

while (<>)
    {
    #  Skip lines with comments or blank lines
    if (/^\s*$/)
	{ next; }
    elsif (/^\s*#/)
	{ next; }
    else
	{
	split;
	$name = @_[0];
	push @names, $name;
	$len = length($name);
	$maxLen = $len if $len > $maxLen;
	$last = $name;
	}
    }

$id = 0;
$width = $maxLen + 3;

foreach $name (@names)
    {
    printf HDR_FILE "#define %-${width}s  %d\n", $name, $id;
    printf SRC_FILE "\t%-${width}s  /* %d */\n",
			"\"$name\"" . (($name eq $last) ? ' ' : ',') , $id;
    $id++;
    }

$id--;

close HDR_FILE;
close SRC_FILE;

exit 0;
