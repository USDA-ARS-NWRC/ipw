#  A Perl script to make an IPW image header from a template.

use Config;

$littleEndian = ($Config{byteorder} =~ /^1234/);
$IPWbyteorder = $littleEndian ? '0123' : '3210';

while (<>) {
    if (/^byteorder =/) {
        $_ = "byteorder = $IPWbyteorder\n";
    }
    elsif (/^!<header> image -1 /) {
	chomp;
	$_ .= "\f\n";
    }
    print;
}
exit 0;
