#  This makefile creates certain data files used by the tests in the parent
#  directory

include $(IPW)/make/_macros

DATA_FILES := elev.78.data

#  Default target is to create data files

.PHONY : data-files clean

data-files : $(DATA_FILES)

elev.78.data : elev.78.asc
	@printf "Making $@\n"
	@$(PERL) Text2bin.pl < $< > $@

clean :
	rm -f $(DATA_FILES)
