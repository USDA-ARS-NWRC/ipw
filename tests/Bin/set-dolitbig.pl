#  A Perl script to set the variable named "dolitbig" in the cmpverb script.

use Config;

$littleEndian = ($Config{byteorder} =~ /^1234/);
$dolitbig = $littleEndian ? 'true' : 'false';

while (<>) {
    if (/^dolitbig=""/) {
        $_ = "dolitbig=\"$dolitbig\"\n";
    }
    print;
}
exit 0;
