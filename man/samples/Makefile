NAME := sample
MAN_SECTION := 1

#  The following 2 macros are defined to show the IPW macros are handled
DESCRIPTION := a sample IPW command to demonstrate IPW man pages
CATEGORY := Foo Modeling

include $(IPW)/make/manpage

$(IPW_MANPG) : $(NAME).src $(IPW_MACRO_FILE)
	grep -v "^<!--" $< > $@
