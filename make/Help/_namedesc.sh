#  shell script to make help file for "_namedesc"
#  $1 = type of object: either "command" or "function"

cat << END
name-desc	Make the text and HTML files with the $1's name and
		  description.
redo-name-desc	Remake the text and HTML files with the $1's name and
		  description.
clean-name-desc	Remove the text and HTML files.
END
