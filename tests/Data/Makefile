#  This makefile creates certain data files used by the tests in the parent
#  directory

include $(IPW)/make/_macros

DATA_FILES := elev.78.hdr elev.78


#  Default target is to create data files

.PHONY : data-files clean

data-files : $(DATA_FILES)

elev.78.hdr : elev.78.hdr-tmpl
	@printf "Making $@\n"
	@$(PERL) MakeHdr.pl < $< > $@

elev.78 : elev.78.hdr ../Outputs/elev.78.data
	@printf "Making $@\n"
	@cat $? > $@

clean :
	rm -f $(DATA_FILES)
