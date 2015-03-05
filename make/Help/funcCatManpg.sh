#  shell script to make help file for "funcCatManpg"

cat << END
all		Same as "man" target (default).
install		Same as "install-man" target.
clean		Same as "clean-man" target.
uninstall	Same as "uninstall-man" target.

list-of-links	Print a list of functions as HTML links (for SEE ALSO section).

END

sh ./_manpage.sh
