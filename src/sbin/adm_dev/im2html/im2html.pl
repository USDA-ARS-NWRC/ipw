# This converts an IPW manual page as printed by the "ipwman" command into
# HTML document that represents an IPW version 2 man page.


#  This variable is used for printing IPW macros (because if we put a
#  macro in a string, it will be replaced when this script is installed).
$percent = "%";

#  Sections for IPW programs

@pgmSections = (
	"NAME",
	"SYNOPSIS",
	"DESCRIPTION",
	"OPTIONS",
	"OPERANDS",
	"EXAMPLES",
	"FILES",
	"DIAGNOSTICS",
	"RESTRICTIONS",
	"HISTORY",
	"BUGS",
	"SEE ALSO",
);

#  Sections for IPW library functions

@funcSections = (
	"NAME",
	"SYNOPSIS",
	"DESCRIPTION",
	"RETURN VALUE",
	"EXAMPLES",
	"GLOBALS READ",
	"GLOBALS MODIFIED",
	"DIAGNOSTICS",
	"RESTRICTIONS",
	"HISTORY",
	"BUGS",
	"SEE ALSO",
);

#  Array of HTML text for each section (indexed by section name)

%sectionText = (
	"NAME"			=> "\n",
	"SYNOPSIS"		=> "\n",
	"DESCRIPTION"		=> "\n",
	"OPTIONS"		=> "<P>\nNone.\n</P>\n",
	"OPERANDS"		=> "<P>\nNone.\n</P>\n",
	"RETURN VALUE"		=> "<P>\nNone.\n</P>\n",
	"EXAMPLES"		=> "\n",
	"FILES"			=> "<P>\nNone.\n</P>\n",
	"GLOBALS READ"		=> "<P>\nNone.\n</P>\n",
	"GLOBALS MODIFIED"	=> "<P>\nNone.\n</P>\n",
	"DIAGNOSTICS"		=> "\n",
	"RESTRICTIONS"		=> "<P>\nNone.\n</P>\n",
	"HISTORY"		=> "\n",
	"BUGS"			=> "<P>\nNone.\n</P>\n",
	"SEE ALSO"		=> "\n",
);


#  Process any options

$name = "";
$numArgs = scalar(@ARGV);
while ($numArgs > 0) {
    $arg = shift(@ARGV);
    $numArgs--;
    if ($arg eq "-f") {
	$fOption = 1;
    } elsif ($arg eq "-c") {
	$cOption = 1;
    } elsif ($arg =~ /^-/) {
	printf STDERR "$0: Error:  Unknown option \"$arg\"\n";
	exit 1;
    } else {
	$name = $arg;
	last;
    }
}  # while

#  Do we extra operands? or -c option without a file name?
if (($numArgs > 0) or ($cOption && $name eq "")) {
    printf STDERR "Usage: $0 [ -f ] [ name ]\n";
    printf STDERR "   or: $0 [ -f ] -c name\n";
    exit 1;
}

#  Program or function man page?
if ($fOption) {
    @sections = @funcSections;
} else {
    @sections = @pgmSections;
}

#  If file name given, open it with appropriate pipe command
if ($name ne "") {
    if ($cOption) {
	#  Strip the man page out of the input
	#  Assume each line of man page comment starts with "**"
	if (! (-f $name and -r _)) {
	    printf STDERR "$0: Error:  Cannot access the file \"$name\"\n";
	    exit 1;
	}
	if (! open STDIN, 
	          "grep '^\\*\\*' $name | sed -e 's/^\*\*      /**	/' -e 's/^\\*\\*//' -e 's/^ //' |") {
	    printf STDERR "$0: Error:  Cannot open the file \"$name\"\n";
	    exit 1;
	}
    } else {
	if (! open STDIN, "%auxDir/ipwman.v1 $name |") {
	    printf STDERR "$0: Error:  Cannot access \"ipwman.v1\" command\n";
	    exit 1;
	}
    }
}

#  First 'section' is to check the first line of the input.
$currSection = "first-line";

SECTION: while ($currSection ne "endOfFile") {

    #  Checking the first line to see if the "ipwman" returned no information
    if ($currSection eq "first-line") {
	#  skipping until next section
	if ($_ = <>) {
	    if (/^No information for:/) {
		#  Just copy the "ipwman" output to standard error, and exit
		print STDERR $_;
		while (<>) {
		    print STDERR $_;
		}
		exit 1;
	    }
	    elsif (/^ ?([A-Z].*\S)/) {
		#  The first line should be the section NAME
		$nextSection = $1;
	    }
	    elsif (/^\s*$/) {
		#  If blank line, then skip to start of next section.
		$nextSection = "skip-section";
	    }
	} else {
	    $nextSection = "endOfFile";
	}
    }

    #  Skipping current section
    elsif ($currSection eq "skip-section") {
	#  skipping until next section
	while (&getSectionLine) {
	    # do nothing
	}
    }

    #  ----------------------------------------------------------
    #  NAME section
    elsif ($currSection eq "NAME") {
	$text = "";
	$name = "";
	while (&getSectionLine) {
	    next if /^\s*$/;
	    s/^\t//;
	    if ($name eq "") {
	    	s/^\s*(\S+)/<B>${percent}name<\/B>/;
	    	$name = $1;
	    }
	    $text .= $_;
	}  # while

	$sectionText{$currSection} = "<P><B>${percent}name</B> - "
				     . "${percent}description</P>\n\n";
    }

    #  ----------------------------------------------------------
    #  SYNOPSIS section
    elsif ($currSection eq "SYNOPSIS") {
	$text = "";
	while (&getSectionLine) {
	    if ($fOption) {
		s/^\t//;
	    } else {
		next if /^\s*$/;
		s/\s{2,}/ /g;
	    }
	    $text .= $_;
	}  # while

	#  The SYNOPIS section becomes PRE-formatted text.
	$rest = $text;
	$text = "<PRE>\n";

	#  For a function man page, just output all the lines.
	if ($fOption) {
	    $text .= $rest;
	    $text .= "</PRE>\n";
	    $sectionText{$currSection} = $text;
	} else {

	    #  Otherwise, for a program man page:
	    #  The first line in this section is left-justified; the
	    #  other lines are indented 5 spaces.  We need to break
	    #  up this section into lines, so we keep track off how
	    #  long the current output line is.  We then see how long
	    #  each option and its argument is, and if it doesn't fit
	    #  fit on the current output line, we start another one.
	    #  Skip up to program name
	    if ($rest =~ /\b($name)\b/) {
		$text .= "<B>${percent}name</B>";
		$rest = $';
		$outLineLen = length($name);
	    }

	    #  Loop while looking for options (-letter or -digit)
	    while ($rest =~ /^\s*(\[\s*)?-([a-zA-Z0-9])/) {
		$required = ($1 eq "");
		$option = $2;
		$rest = $';
		if ($required) {
		   if ($rest =~ /^\s*\[\s*(\S+)\s*\]/) {  # optional arg
			$argList = "[". $1 . "]";
			$rest = $';
		   }
		   else {  # required argument
			$rest =~ /^\s*(\S+)/;
		  	$argList = $1;
			$rest = $';
		   }
		}
		else {  #  optional option
		    if ($rest =~ /^\s*\[\s*(\S+)\s*\]\s*\]/) {  # opt'l arg
			$argList = "[". $1 . "]";
			$rest = $';
		    }
		    else {  # required argument or no argument
			$rest =~ /^\s*((\S+)\s*)?\]/;
		  	$argList = $1;
			$rest = $';
		    }
		}

		#  The length of the option and its argument-list
		$optLen = 3;  # space & -option
		$optLen += 1 + length($argList) if length($argList) > 0;
		$optLen += 4 if ! $required;  #  for the brackets & spaces

		#  Surround each "word" in argument-list with italics
		$argList =~ s/([^,\[\]]+)/<I>$1<\/I>/g;

		push(@options, $option);
		$optionArgs{$option} = $argList;

		if ($outLineLen + $optLen > 60) {
		    $text .= "\n     ";
		    $outLineLen = 5;
		}
		$outLineLen += $optLen;

		if ($required) {
		    $text .= " <B>-$option</B>";
		    $text .= " $argList" if length ($argList) > 0;
		} else {
		    $text .= " [ <B>-$option</B>";
		    $text .= " $argList" if length ($argList) > 0;
		    $text .= " ]";
		}
	    }  # while looking for options

	    #  Remaining words must be operands; surround them with italics
	    $operands = $rest;
	    $operands =~ s/^\s+//;
	    $operands =~ s/\s+$//;
	    if ($outLineLen + 1 + length($operands) > 60) {
		$text .= "\n     ";
	    }
	    $operands = &doSpecChars($operands);
	    $operands =~ s/([^,\[\]]+)/<I>$1<\/I>/g;

	    $text .= " $operands\n";
	    $text .= "</PRE>\n";
	    $sectionText{$currSection} = $text;

	    $reploptions = "";
	    foreach $opt (@options) {
		$opt =~ s/^(.|\n)-//;
		$reploptions .= "s/(^|[^a-zA-Z0-9])-$opt([^a-zA-Z0-9])/" .
				"\$1<B>-$opt<\\/b>\$2/g;\n";
	    }
	} # else
    }

    #  ----------------------------------------------------------
    #  DESCRIPTION section
    #  EXAMPLES section
    #  RESTRICTIONS section
    #  BUGS section
    #  RETURN VALUE section
    #  GLOBAL* sections
    #
    #  (Note:  these sections are handled in the same way)
    elsif ($currSection eq "DESCRIPTION" or
		$currSection eq "EXAMPLES" or
		$currSection eq "RESTRICTIONS" or
		$currSection eq "BUGS" or
		$currSection eq "RETURN VALUE" or
		$currSection =~ /GLOBAL/) {
	if ($currSection =~ /GLOBAL/) {
	    if ($currSection =~ /ACCESSED/ ||
			$currSection =~ /READ/) {
		$currSection = "GLOBALS READ";
	    } else {
		$currSection = "GLOBALS MODIFIED";
	    }
	}
	$text = "";
	$block = "";
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    ($block, $line) = &doTextBlocks($block);
	    $text .= $line;
	}  # while

	$text .= "</$block>" if $block ne "";
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  OPTIONS section
    elsif ($currSection eq "OPTIONS") {
	$text = "";
	$inOption = 0;
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    s/^\t//;

	    #  Do we have the start of an option's description?
	    if (/^-.\s+/) {
		($opt) = /^-(.)\s+/;
		s/^-(.)\s+//;

		#  If we were already in another option, then put a 
		#  paragraph tag before the next option.
		if ($inOption) {
		    $text .= "\n<P>\n";
		} else {
		    #  Start a new definition-list for the following options
		    $text .= "\n<DL>\n";
		    $inOption = 1;
		}

		#  Start a separate definition for the option
		$text .= "<DT><B>-$opt</B> " . $optionArgs{$opt} . "\n";
		$text .= "<DD>\n";
		$endTag = "</DD>";
		$newP = 0;
	    }

	    #  Or, do we have just independent text that's justified with
	    #  options?
	    elsif (/^\S/) {
		#  If we were already in an option, then wrap it up.
		if ($inOption) {
		    $text .= "</DL>\n";
		}
		$inOption = 0;
		$text .= "\n<P>";
		$endTag = "</P>";
		$newP = 0;
	    }

	    #  Or, do we have a blank line?  This denotes the end of current
	    #  paragraph of text.
	    elsif (/^\s*$/) {
		$text .= "$endTag\n" if $endTag ne "";
		$newP = 1;
	    }

	    #  We have indented text which is part of an option's description.
	    else {
		#  Are we starting a new paragraph?
		if ($newP) {
		    $text .= "\n<P>";
		    $newP = 0;
		    $endTag = "</P>";
		}
	    }

	    s/\b($name)\b/<B>${percent}name<\/B>/g;
	    s/\{([^\s}]+)\}/<I>$1<\/I>/g;
	    eval $reploptions;
	    s/^\s+//;
  
	    $text .= $_;
	}  # while

	#  If we are in an option, then wrap it up.
	if ($inOption) {
	    $text .= "</DL>\n";
	}
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  OPERANDS section
    #  (Also called OPERAND)
    elsif ($currSection eq "OPERANDS" or
		$currSection eq "OPERAND") {
	if ($currSection eq "OPERAND") {
	    $currSection = "OPERANDS";
	}

	#  If we have operands from SYNOPSIS, then utilize a definition-list.
	if ($operands ne "") {
	    $text = "<DL>\n";
	    $text .= "<DT>$operands</DT>\n\n";
	    $text .= "<DD>";
	    $tag = "DD";
	} else {
	    $text = "";
	    $tag = "";
	}
	$foundText = 0;
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    if (/\S/) {
		#  We have a non-blank line.
	        if ($tag eq "") {
		    $text .= "<P>";
		    $tag = "P";
		}
		$foundText = 1;
	    } else {
		#  We have a blank line.  If we already found some text, then
		#  end that previous text block, and start a <P> text block.
		if ($foundText) {
		    $text .= "</$tag>";
		    $text .= "<P>";
		    $tag = "P";
		}
	    }
	    $text .= $_;
	}  # while

	if ($tag ne "") {
	    $text .= "</$tag>\n";
	}
	if ($operands ne "") {
	    $text .= "</DL>\n";
	}
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  FILES section
    elsif ($currSection eq "FILES") {
	#  This section is just PRE-formatted text indented 5 spaces
	$text = "<PRE>\n";
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    if (/^\s*$/) {
		next;
	    }
	    s/^\t/     /;
	#    s/\b($name)\b/<B>${percent}name<\/B>/g if $indent;
	    s/\{([^\s}]+)\}/<I>$1<\/I>/g;
  
	    $text .= $_;
	}  # while

	$text .= "</PRE>\n";
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  DIAGNOSTICS section
    #  ERROR(S) section
    elsif ($currSection eq "DIAGNOSTICS" or
	   $currSection =~ /ERROR/) {

	if ($currSection =~ /ERROR/) {
	    $currSection = "DIAGNOSTICS";
	}

	#  In IPW man pages, the diagnostic messages are not indented, but
	#  their explanations that follow them are indented.
	#  Messages are placed in a definition list like the options.

	$text = "";
	$tag = "";
	$msgCount = 0;
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    #  A blank line marks the end of current tag'ed block
	    if (/^\s*$/) {
		$text .= "</$tag>\n" if $tag ne "";
		$tag = "";
		next;
	    }

	    s/^\t {0,2}//;
	    if (/^\S/) {
		#  We have a diagnostic message
		$msgCount++;
		if ($msgCount == 1) {
		    $text .= "<DL>\n";
		} 
		if ($tag ne "DT") {
		    $text .= "</$tag>\n" if $tag ne "";
		    $text .= "\n<P>\n<DT>";
		    $tag = "DT";
		}
	    } else {
		#  We have the explanation for a message
		if ($tag ne "DD") {
		    $text .= "</$tag>\n" if $tag ne "";
		    $text .= "<DD><BR>";
		    $tag = "DD";
		}
	    }
	    s/\b($name)\b/<B>${percent}name<\/B>/g if $tag ne "DT";
	    s/\{([^\s}]+)\}/<I>$1<\/I>/g;
	    eval $reploptions;
  
	    $text .= $_;
	}  # while

	$text .= "</$tag>\n" if $tag ne "";
	$text .= "</DL>\n" if $msgCount > 0;
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  HISTORY section
    elsif ($currSection eq "HISTORY") {
	#  convert each history entry into a separate definition list
	$text = "";
	$inEntry = 0;
	@months = ("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
		   "Sep", "Oct", "Nov", "Dec");
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    #  A blank line ends the current history entry
	    if (/^\s*$/) {
		$text .= "</DD>\n</DL>\n" if $inEntry;
		$inEntry = 0;
		next;
	    }

	    s/^\t {0,2}//;

	    #  Potential date formats: 11/2/84
	    #			       11/84
	    #			       Nov 84
	    #			       June 6, 1995
	    #			       June 6 1995
	    #			       Jun 1995
	    #  Each of these can be followed by a colon, e.g., Nov 84:
	    if (/^\S/) {
		$date = "";
		if (/^(\d+)\/(\d+)\/(\d+):?/) {   # 11/2/84
			$mon = $months[$1-1];
			$year = $3;
		} elsif (/^(\d+)\/(\d+):?/) {   # 11/84
			$mon = $months[$1-1];
			$year = $2;
		} elsif (/^(\w)(\w+)\s+(\d+),?\s+(\d+):?/) {   # June 6, 1984
			$monFirst = $1;
			$monFirst =~ tr/a-z/A-Z/;
			$monRest = substr($2, 0, 2);
			$monRest =~ tr/A-Z/a-z/;
			$mon = $monFirst . $monRest;
			$year = $4;
		} elsif (/^(\w)(\w+),?\s+(\d+):?/) {   # November 84 or Nov 1984
			$monFirst = $1;
			$monFirst =~ tr/a-z/A-Z/;
			$monRest = substr($2, 0, 2);
			$monRest =~ tr/A-Z/a-z/;
			$mon = $monFirst . $monRest;
			$year = $3;
		} else {	#  Assume date extends until tab character
			/^(\S[^\t]*)/;
			$date = $1;
		}

		$_ = $';
		if ($date eq "") {
		    $year += 1900 if $year < 100;
		    $date = "$mon $year";
		}

		#  End any previous entry
		$text .= "</DD>\n</DL>\n" if $inEntry;

		$text .= "\n<DL>\n";
		$text .= "<DT>$date</DT>\n";
		$text .= "<DD>\n";
		$inEntry = 1;
	    } else {
		#  Assume this line is just a continuation of the current
		#  entry.
		s/^\s+//;
	    }

	    s/\b($name)\b/<B>${percent}name<\/B>/g;
	    s/\{([^\s}]+)\}/<I>$1<\/I>/g;
	    eval $reploptions;
  
	    $text .= $_;
	}  # while

	$text .= "</DD>\n</DL>\n" if $inEntry;
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  SEE ALSO section
    elsif ($currSection eq "SEE ALSO") {
	#  IPW man page might have
	#	IPW: foo, bar, ...
	#	Unix: cat, dog, ...
	#
	#	(any then some references)
	#
	#  The IPW and Unix lists should be put into separate definition
	#  lists.  References are put into paragraphs.
	$text = "";
	$tag = "";
	$list = "";
	while (&getSectionLine) {
	    $_ = &doSpecChars($_);
	    #  A blank line marks the end of current tag'ed block
	    if (/^\s*$/) {
		$text .= "</$tag>\n" if $tag ne "";
		$text .= "</DL>\n" if $tag eq "DD";
		$tag = "";
		$list = "";
		next;
	    }

	    s/^\t//;
	    if (/^(\w+):/) {
		#  Start of a list, either IPW: or Unix:
		#  End any previous list
		if ($list ne "") {
		    $text .= "</DD>\n";
		    $text .= "</DL>\n";
		}

		#  Start a new definition list
		$list = $1;
		s/^(\w+):\s+/\t/;	#  Need tab for "if" statement below
		$text .= "\n<DL>\n";
		$text .= "<DT>$1</DT>\n";
		$text .= "<DD>";
		$tag = "DD";
	    }

	    #  Non-indented text is a reference.
	    if (/^\S/) {
		if ($tag ne "P") {
		    $text .= "</$tag>\n" if $tag ne "";
		    $text .= "</DL>\n" if $tag eq "DD";
		    $text .= "<P>";
		    $tag = "P";
		    $list = "";
		}
	    } else {
		#  Indented text; assume it belongs to previous list, if any.
		#  Otherwise, treat a though it were a reference.
		if ($list eq "" and $tag ne "P") {
		    $text .= "</$tag>\n" if $tag ne "";
		    $text .= "</DL>\n" if $tag eq "DD";
		    $text .= "<P>";
		    $tag = "P";
		}
	    }

	    #  If we're in a IPW list, convert each item to a link
	    if ($list eq "IPW") {
		s/\b([^\s,]+)\b/<B><A href="$1.html">$1<\/A><\/B>/g;
		s/,\s*/,\n/g;
	    }
  
	    $text .= $_;
	}  # while

	$text .= "\n</$tag>\n" if $tag ne "";
	$text .= "\n</DL>\n" if $tag eq "DD";
	$sectionText{$currSection} = $text;
    }

    #  ----------------------------------------------------------
    #  Skip the FUTURE DIRECTIONS section (it usually just mentions
    #  that a command is a script, and maybe should be a program;
    #  not really important).  Those rare things that do with potential
    #  changes for the future should be noted in a separate place, i.e.,
    #  an overall "To Do" list for IPW.
    elsif ($currSection eq "FUTURE DIRECTIONS") {
	#  Just skip it
	$nextSection = "skip-section";
    }

    #  ----------------------------------------------------------
    #  Last line at bottom of IPW man page starts with
    #	 "IPW ERLC Extension Program  ..."
    #  or
    #	 "IPW UCSB Extension Program  ..."
    elsif ($currSection =~ /\s*IPW\s/) {
	#  Just skip it
	$nextSection = "skip-section";
    }

    #  ----------------------------------------------------------
    #  Unknown section
    else {
	print STDERR "Warning: skipping unknown section: $currSection\n";
	$nextSection = "skip-section";
    }

    $currSection = $nextSection;
}  #  end of 'while' loop reading the input file (SECTION)


#  output the sections of the manual page
foreach $section (@sections) {
   print "<H2>$section</H2>\n\n";
   if ($section eq "OPERANDS" and $sectionText{$section} eq "\n") {
	print "$operands\n";
   } else {
	print $sectionText{$section};
	print "\n";
   }
}

exit 0;


#  --------------------------------------------------------------------------

#  This subroutine gets the next line in the current section from standard
#  input.
#
#  Arguments
#	none
#
#  Return value
#	1	the next line has been read into the variable $_
#
#	0	the start of the next section was found, or the end of
#		file was found.  In the former case, the variable
#		$nextSection has the name of the next section.  In the
#		latter case, the variable $nextSection is set to "endOfFile".

sub getSectionLine {

    if ($_ = <>) {
	if (/^ ?([A-Z].*\S)/) {
	    $nextSection = $1;
	    0;
	} else {
	    1;
	}
    } else {
	$nextSection = "endOfFile";
	0;
    }
}

#  --------------------------------------------------------------------------

#  This subroutine replaces certain special characters with their HTML
#  entity notation.
#
#  Arguments
#	text:	string of text to scan
#
#  Return value
#	the argument string with all special characters replaced

sub doSpecChars {
    local ($text) = @_;

    $text =~ s/&/&amp;/g;
    $text =~ s/</&lt;/g;
    $text =~ s/>/&gt;/g;
    $text =~ s/"/&quot;/g;

    $text;
}

#  --------------------------------------------------------------------------

#  This subroutine processes a single input line to determine if there
#  should be a switch in the type of text block.
#
#  Arguments
#	block:	current text block type (either "P" or "PRE" or "")
#	$_:	current input line
#
#  Return values
#	block:	updated text block type (either "P" or "PRE" or "")
#	line:	updated input line (possibly with leading HTML tags)

sub doTextBlocks {
    local ($block) = @_;
    local ($line) = "";

    if (/^\s*$/) {
	#  A blank line ends the current text block
	$line .= "</$block>" if $block ne "";
	$block = "";
    } else {
	s/^\t//;
	if (/^\s/) {
	    #  Indented text line -> treat a PRE-formated text
	    if ($block ne "PRE") {
		$line .= "</$block>\n" if $block eq "P";
		$block = "PRE";
		$line .= "<$block>\n";
	    }
	} else {
	    #  Normal text block
	    if ($block ne "P") {
		$line .= "</$block>\n" if $block eq "PRE";
		$block = "P";
		$line .= "<$block>\n";
	    }

	    s/\b($name)\b/<B>${percent}name<\/B>/g;
	    s/\{([^\s}]+)\}/<I>$1<\/I>/g;
	    eval $reploptions;
	}
	$line .= $_;
    }

    ($block, $line);
}
