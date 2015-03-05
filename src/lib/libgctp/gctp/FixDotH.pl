#! /usr/local/bin/perl

#  This perl script fixes the nad27.h and nad83.h files.  GCC with the -Wall
#  option will yield warnings about not having braces around the initializers
#  for structure members "fields".  This member is a 9-element array.
#
#  To fix the warnings, this program places braces around the 9 numeric
#  values for each structure in the array defined in each file.  This
#  program assumes the file has a regular structure as follows:
#
#	initial lines with comments
#
#	array declaration, e.g.: 
#		NadRec Nad27[] = {
#
#	one or more structure initializers; each initializer is 3 lines:
#		{ string 			, integer , string , integer,
#		  real,			real,			real,
#		  real,			real,			real,
#		  real,			real,			real},
#
#	the ending of the array declaration, i.e.:
#		};
#

$insideStruct = 0;

while (<>)
	{
	if (! $insideStruct)
		{
		print;
		$insideStruct = 1, $structLine = 1 if /^\{ "/
		}
	else
		{
		$structLine++;
		if ($structLine == 2)
			{
			print " {$_";
			}
		elsif ($structLine == 3)
			{
			print "  $_";
			}
		else
			{
			s/\}/\}\}/;
			print "  $_";
			$insideStruct = 0;
			}
		}
	}
