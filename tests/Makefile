#  IPW's test directory.

#  The current path with any IPW directories stripped out

NO_IPW_PATH := $(shell printf "$$PATH\n" | sed "s=[^:]*/ipw/[^:]*:==g")

# This makes sure we are testing the correct commands.

PATH := $(IPW)/bin:$(IPW)/sbin:$(IPW)/local/bin:$(IPW)/local/sbin:$(IPW)/tests/Bin:$(NO_IPW_PATH)

# I would like to do an 'export PATH' here, but some people may have
# GNU Make version 3.62, which doesn't understand such things.
.EXPORT_ALL_VARIABLES:

# Include the configuration macros (GRAPH)
include $(IPW)/conf/conf.mk

# Environment variables to enable checks on malloc() and free()
MALLOC_CHECKS := MallocBadFreeAbort=1

# On OS X 10.6 (Snow Leopard) with Xcode 3.2, there's a subtle path bug with
# GNU Make and /bin/sh (IPW ticket #22).  Workaround: use Bash shell.
ifeq ($(shell uname -s),Darwin)
  OS_X_VERSION := $(shell sw_vers -productVersion)
  ifeq ($(findstring 10.6.,$(OS_X_VERSION)),10.6.)
    SHELL := /bin/bash
  endif
endif

#  "Test" for a command that isn't practical to test

define NOT_TESTING
@printf "    Not testing command: $(@)\n"
@touch $@
endef

#  Test for a command that hasn't been implemented yet

define NO_TEST_YET
@printf "    No test for command: $(@) (test not yet developed)\n"
@touch $@
endef

# ---------------------------------------------------------------------------

.PHONY : scripts-data-test scripts data-files clean distclean

#  First make the scripts and the data files used in tests

scripts-data-test : scripts data-files all

scripts :
	$(MAKE) -C Bin

data-files :
	$(MAKE) -C Outputs
	$(MAKE) -C Data

clean :
	rm -f [a-z]*
	rm -f Categories/*

distclean : clean
	$(MAKE) -C Bin clean
	$(MAKE) -C Outputs clean
	$(MAKE) -C Data clean
	
# ---------------------------------------------------------------------------

#  NOTE:  The order of commands is important.  Since some commands
#	  use other commands, the more basic or stand-alone commands
#	  should be tested first.  Even the commands used by the
#	  testing command "cmpverb" (i.e., "striphist") uses some
#	  basic IPW commands (e.g., "prhdr" and "rmhdr").  So these
#	  IPW commands should be tested before any tests that use
#	  the -h option with "cmpverb".  That's why the "basic"
#	  and "header" categories are first.

CATEGORIES := basic header datetime numeric climate import export lq \
	      transform pixcalc stats classify topocalc proj phys.pt \
	      phys.spat physmod.pt physmod.spat rad.pt rad.bas rad.glb misc

CATEGORY_FILES := $(foreach category, $(CATEGORIES), Categories/$(category))

.PHONY : all Intro

all : Intro $(CATEGORY_FILES)
	@printf "\nTesting done\n"

Intro :
	@printf "\nTesting IPW commands\n"

$(CATEGORY_FILES) : Categories/% : %.Intro %.Cmds

#  The @ before a command suppresses echoing of the command line
#  The - before a command indicates that make should continue even if
#    an error happens

# ---------------------------------------------------------------------------

CATEGORY_INTROS := $(foreach category, $(CATEGORIES), $(category).Intro)

.PHONY : $(CATEGORY_INTROS)

$(CATEGORY_INTROS) : %.Intro :
	@if [ ! -r Categories/$* ] ; then \
	  case $* in \
	    basic)	  hdr='Basic Image Commands';;\
	    datetime)	  hdr='Dates and Times';;\
	    numeric)	  hdr='Numeric Data in Text Files';;\
	    climate)	  hdr='Climate Data in Text Files';;\
	    import)	  hdr='Importing Image Data';;\
	    export)	  hdr='Exporting IPW Image Data';;\
	    header)	  hdr='Image Header Manipulation';;\
	    lq)		  hdr='Linear Quantization';;\
	    transform)	  hdr='Image Transformation';;\
	    pixcalc)	  hdr='Pixel Calculations';;\
	    stats)	  hdr='Image Statistics';;\
	    classify)	  hdr='Image Classification';;\
	    topocalc)	  hdr='Topographic Calculations';;\
	    proj)	  hdr='Geographic (Map) Projections';;\
	    phys.pt)	  hdr='Environmental Physics and Properties: Point';;\
	    phys.spat)	  hdr='Environmental Physics and Properties: Spatial';;\
	    physmod.pt)	  hdr='Physical Modeling: Point';;\
	    physmod.spat) hdr='Physical Modeling: Spatial';;\
	    rad.pt)	  hdr='Radiation Modeling: Point';;\
	    rad.bas)	  hdr='Radiation Modeling: Basin-Scale';;\
	    rad.glb)	  hdr='Radiation Modeling: Global-Scale';;\
	    misc)	  hdr='Miscellaneous';;\
	  esac ; \
	  printf "\n$$hdr\n\n" ; \
	fi

define CHECK_CMDS
@( $(foreach cmd,$^, test -r $(cmd) &&) touch Categories/$(@:.Cmds=) ) || exit 0
endef

# ---------------------------------------------------------------------------

basic.Cmds : ipwfile primg mux demux cmpimg poly
	$(CHECK_CMDS)

ipwfile :
	@-ipwfile Data/ch1.ipw Data/ch5.ipw Data/avhrr-elev.ipw \
	          Data/horizons.78 Data/ea.78 >ipwf-all
	@cmpverb ipwfile ipwf-all

primg :
	@-primg -r -a -i Data/elev.78 > elev.78.asc
	@cmpverb primg elev.78.asc

mux :
	@-mux Data/elev.78 Data/mask.alb.78 > elevmask.ipw
	@cmpverb -n mux elevmask.ipw
	@-mux Data/ch1.ipw Data/ch2.ipw | mux - Data/ch5.ipw > channels.ipw
	@cmpverb mux channels.ipw

demux :
	@-demux -b 1,0 Outputs/elevmask.ipw > maskelev.ipw
	@cmpverb -n demux maskelev.ipw
	@-demux -b 0- Outputs/channels.ipw > channels.ipw
	@cmpverb -h demux channels.ipw

cmpimg :
	@-cmpimg Data/ch1.ipw Data/ch1.ipw  > cmpr
	@-cmpimg Data/ch1.ipw Data/ch2.ipw  >> cmpr
	@cmpverb cmpimg cmpr

poly :
	@-poly < Data/poly1 > opoly1
	@cmpverb -n poly opoly1
	@-poly -s 15,15 < Data/poly2 > opoly2
	@cmpverb poly opoly2

# ---------------------------------------------------------------------------

header.Cmds : prhdr rmhdr mkbih mklqh mkgeoh mkwinh mkcrh mksunh mksath edhdr
	$(CHECK_CMDS)

prhdr :
	@-prhdr Data/ch1.ipw > ch1.hdr
	@cmpverb prhdr ch1.hdr

rmhdr :
	@-rmhdr Data/elev.78 > elev.78.data
	@cmpverb rmhdr elev.78.data

mkbih :
	@-mkbih -s 1 -l 1 -i 10 -f > head0
	@cmpverb -n mkbih head0
	@-mkbih -l 64 -s 64 -f > head64
	@cmpverb -n mkbih head64
	@-mkbih -l 512 -s 512 -f > head512
	@cmpverb mkbih head512

mklqh :
	@-mklqh -m 0,0,1023,1 -f < Outputs/head0 > head1
	@cmpverb -n mklqh head1
	@-mklqh -m 0,462,1023,895 -u "W m^-2" -f < Outputs/head0 > head3
	@cmpverb mklqh head3

mkgeoh :
	@-mkgeoh -c utm -u meters -o 4051800,349350 -d 5,5 -f \
	 < Outputs/head1 > head2
	@cmpverb mkgeoh head2

mkwinh :
	@-mkwinh -b 50,70 Data/ch1.ipw > ch1-5070.ipw
	@cmpverb mkwinh ch1-5070.ipw

mkcrh :
	@-mkcrh -f -c Data/classvals -i Data/rad.10.ipw > col.crh
	@cmpverb mkcrh col.crh

mksunh :
	@-mksunh -z 0.838671 -a 1.064651 -f < Outputs/head4 > head5
	@cmpverb mksunh head5

mksath :
	@-mksath -p EOS1 -s TM -l "North America" -d 19921201 -t 221510.213 -f \
	 < Outputs/head3 > head4
	@cmpverb mksath head4

edhdr :
	$(NOT_TESTING)

# ---------------------------------------------------------------------------

datetime.Cmds : addtime cvtime
	$(CHECK_CMDS)

addtime :
	@-addtime -d 1990,6,22,9,0,0 -h 4.6 > addtime-jun22
	@cmpverb addtime addtime-jun22

cvtime :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

numeric.Cmds : seq random smooth akintrp interp ezint qdint
	$(CHECK_CMDS)

seq :
	@-seq -r 200,600 -n 4       > seq1
	@cmpverb -n seq seq1
	@-seq -r 200,600 -d 100     > seq2
	@cmpverb -n seq seq2
	@-seq -r 0,10 -n 30 -s exp  > seq3
	@cmpverb seq seq3

random :
	@-random -s 5 -n 100 -r 0,100,0,100 > random500
	@cmpverb random random500

smooth :
	$(NO_TEST_YET)

akintrp :
	$(NO_TEST_YET)

interp :
	@-interp < Data/interp1 > int1
	@cmpverb -n interp int1
	@-interp < Data/interp2 > int2
	@cmpverb interp int2

ezint :
	$(NO_TEST_YET)

qdint :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

climate.Cmds : hcnx proport
	$(CHECK_CMDS)

hcnx :
	$(NO_TEST_YET)

proport :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

import.Cmds : text2ipw text2bin pbm2ipw dem2ipw grid2ipw surface msurface \
		msurfprep crossval tmpt
	$(CHECK_CMDS)

text2ipw :
	$(NO_TEST_YET)

text2bin :
	@-text2bin -2 Outputs/elev.78.asc > elev.78.data
	@cmpverb text2bin elev.78.data

pbm2ipw :
	@-pbm2ipw Outputs/ndvi.ppm >ndvi-ppm.ipw
	@cmpverb -n pbm2ipw ndvi-ppm.ipw
	@-pbm2ipw Outputs/ndvig.pgm >ndvi-pgm.ipw
	@cmpverb pbm2ipw ndvi-pgm.ipw

dem2ipw :
	$(NO_TEST_YET)

grid2ipw :
	$(NO_TEST_YET)

surface :
	$(NO_TEST_YET)

msurface :
	$(NO_TEST_YET)

msurfprep :
	$(NO_TEST_YET)

crossval :
	$(NO_TEST_YET)

tmpt :
	$(NOT_TESTING)

# ---------------------------------------------------------------------------

export.Cmds : ipw2bin ipw2ps ipw2sun ipw2grid ipw2pbm ipw2pgm ipw2ppm ipw2hds \
		ipw2xim bin2text
	$(CHECK_CMDS)

ipw2bin :
	$(NOT_TESTING)

ipw2ps :
	@-ipw2ps Data/ch2.ipw > ch2.ps
	@cmpverb -n ipw2ps ch2.ps
	@-ipw2ps -r Data/ch2.ipw > ch2r.ps
	@cmpverb ipw2ps ch2r.ps

ipw2sun :
	@-ipw2sun Outputs/ch1-d8.ipw > ch1-d8.ras
	@cmpverb -n ipw2sun ch1-d8.ras
	@-ipw2sun Data/ch2.ipw > ch2.ras
	@cmpverb ipw2sun ch2.ras

ipw2grid :
	$(NO_TEST_YET)

ipw2pbm :
	@-ipw2pbm Data/ch2.ipw > ch2.pbm
	@cmpverb ipw2pbm ch2.pbm

ipw2pgm :
	@-ipw2pgm Data/ndvi.ipw > ndvig2.pgm
	@cmpverb ipw2pgm ndvig2.pgm

ipw2ppm :
	@-ipw2ppm -c Data/ndvi.cm Data/ndvi.ipw > ndvi.ppm
	@cmpverb -n ipw2ppm ndvi.ppm
	@-ipw2ppm -b -g Data/ndvi.ipw > ndvig.pgm
	@cmpverb ipw2ppm ndvig.pgm

ipw2hds :
	@-ipw2hds < Outputs/ch1-d8.ipw > ch1-d8.hds
	@cmpverb ipw2hds ch1-d8.hds

ipw2xim :
	@-ipw2xim Data/ch2.ipw > ch2.xim
	@cmpverb ipw2xim ch2.xim

bin2text :
	@-bin2text -2 Outputs/elev.78.data > elev.78.asc
	@cmpverb bin2text elev.78.asc

# ---------------------------------------------------------------------------

lq.Cmds : lqmm requant lqhx pix2fp
	$(CHECK_CMDS)

lqmm :
	$(NO_TEST_YET)

requant :
	@-requant -b 8 Data/avhrr-elev.ipw > avelevb8.ipw
	@cmpverb -n requant avelevb8.ipw
	@-requant -m -500,10000 Data/avhrr-elev.ipw > avelevmc.ipw
	@cmpverb -e requant avelevmc.ipw

lqhx :
	@-lqhx -h Data/rad.10.ipw -i Data/rad.04.ipw > rad.04.res.ipw
	@cmpverb -n lqhx rad.04.res.ipw
	@-lqhx -i Data/ch1.ipw < Outputs/head1 > ch1-lq.ipw
	@cmpverb lqhx ch1-lq.ipw

pix2fp :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

transform.Cmds : flip rotate skew transpose resamp window zoom randomize \
		 convolve fillbands dither wedge edimg
	$(CHECK_CMDS)

flip :
	@-flip -l -s Data/ch1.ipw > ch1-180.ipw
	@cmpverb flip ch1-180.ipw

rotate :
	@-rotate -a 14 Data/ndvi.ipw >ndvi14.ipw
	@cmpverb rotate ndvi14.ipw

skew :
	@-skew -a 30 Data/ch1.ipw > ch1-skew30.ipw
	@cmpverb skew ch1-skew30.ipw

transpose :
	@-transpose Data/ch1.ipw > ch1t.ipw
	@cmpverb -n transpose ch1t.ipw
	@-transpose Data/ch1.ipw | transpose | cmpimg - Data/ch1.ipw > cmptr
	@cmpverb transpose cmptr

resamp :
	@-resamp -s 20 -l 4 Data/ch1.ipw  > ch1-resamp1.ipw
	@cmpverb -n resamp ch1-resamp1.ipw
	@-resamp -s 5 -l 5 -f max Data/ch2.ipw >ch2-resamp2.ipw
	@cmpverb resamp ch2-resamp2.ipw

window :
	@-window -c 48,140 -n 48,140 Data/ch1.ipw  > ch1-win.ipw
	@cmpverb -n window ch1-win.ipw
	@-window -g 0 -b 2875000,-1655000 -e 2825000,-1605000 Data/elev.78 \
          > elev-gwin.ipw
	@cmpverb -n window elev-gwin.ipw
	@-$(MALLOC_CHECKS) window -b 1,2 -n 3,4 Data/small.ipw  > small-win.ipw
	@cmpverb window small-win.ipw

zoom :
	@-zoom -s -20 -l -4 Data/ch1.ipw  > ch1-zoom1.ipw
	@cmpverb -n zoom ch1-zoom1.ipw
	@-zoom -s 2 -l 3 Outputs/ch1-zoom1.ipw  > ch1-zoom2.ipw
	@cmpverb -n zoom ch1-zoom2.ipw
	@-zoom -s 20 -l -40 Outputs/channels.ipw  > channels-zoom.ipw
	@cmpverb zoom channels-zoom.ipw

randomize :
	@-randomize -s 5 Outputs/ch1.hdr  > ch1.hdr-rand
	@cmpverb -n randomize ch1.hdr-rand
	@-cat Outputs/ch1.hdr | randomize -s 5  > ch1.hdr-rand
	@cmpverb randomize ch1.hdr-rand

convolve :
	@-convolve -i Data/ndvi.ipw -c Data/conv.laplacian > lndvi.ipw
	@cmpverb -e convolve lndvi.ipw

fillbands :
	$(NO_TEST_YET)

dither :
	@-dither -r 8 Data/ch1.ipw > ch1-d8.ipw
	@cmpverb dither ch1-d8.ipw

wedge :
	@-wedge -c 1,1 -n 8 < Outputs/head64 > wedge64.ipw
	@cmpverb -n -e wedge wedge64.ipw
	@-wedge -c 0,1,0,-1 -n 12 Data/elev.78 > wedge-elev.ipw
	@cmpverb -e wedge wedge-elev.ipw

edimg :
	@-edimg -c Data/edimg-coords -i Data/ndvi.ipw > ndvi-ed.ipw
	@cmpverb edimg ndvi-ed.ipw

# ---------------------------------------------------------------------------

pixcalc.Cmds : imath imgsum normalize iinterp ninterp despike bitcom \
		iadd mult scale lincom itrim
	$(CHECK_CMDS)

imath :
	$(NO_TEST_YET)

imgsum :	
	@-imgsum -m Data/mask.alb.78 Data/elev.78 > elev78-sum
	@cmpverb imgsum elev78-sum

normalize :
	$(NO_TEST_YET)

iinterp :
	$(NO_TEST_YET)

ninterp :
	$(NO_TEST_YET)

despike :
	$(NO_TEST_YET)

bitcom :
	@-bitcom -a -m  < Outputs/elevmask.ipw > maskedelev.ipw
	@cmpverb bitcom maskedelev.ipw

iadd :
	@-iadd Outputs/grad.78 > iadd-grad
	@cmpverb iadd iadd-grad

mult :
	@-mult Outputs/grad.78  > mult-grad
	@cmpverb -n -e mult mult-grad
	@-mult -r 2 < Outputs/channels.ipw > mult-chans
	@cmpverb -e mult mult-chans

scale :
	$(NO_TEST_YET)

lincom :
	@-lincom < Outputs/grad.78 > gradave
	@cmpverb -n -e lincom gradave
	@-lincom -c 1,-1 < Outputs/grad.78 > gradsub
	@cmpverb -n -e lincom gradsub
	@-lincom -c .5,.5,1,-1 < Outputs/grad.78 > gradsumdiff
	@cmpverb -e lincom gradsumdiff

itrim :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

stats.Cmds : imgstat mstats istatrel istatgraph ipwinterp hist grhist histeq \
		cnhist cnhist.all lutx mklut
	$(CHECK_CMDS)

imgstat :
	@-imgstat Data/ch1.ipw >ch1.stats
	@cmpverb -n imgstat ch1.stats
	@-imgstat -c Data/ndc.ipw >ndvi.clsstats
	@cmpverb imgstat ndvi.clsstats

mstats :
	@-mstats -c Data/avhrr-mask.ipw Outputs/channels.ipw > chanstats
	@cmpverb mstats chanstats

istatrel :
	@-istatrel Data/ch1.ipw Data/ch2.ipw > ch1-2
	@cmpverb istatrel ch1-2

istatgraph :
	$(NOT_TESTING)

ipwinterp :
	@-ipwinterp Data/ch1.ipw < Outputs/ch1-2 > ch2i.ipw
	@cmpverb -e ipwinterp ch2i.ipw

hist :
	@-hist -m Data/mask.alb.78 Data/elev.78 > emhist
	@cmpverb -n hist emhist
	@-hist Data/ndvi.ipw > ndvihist
	@cmpverb hist ndvihist


ifeq ($(GRAPH),)
grhist :
	@printf "    Not testing command: $(@) (graph command not found)\n"
	@touch $@
else
  GNU_GRAPH := $(shell $(GRAPH) --version </dev/null 2>/dev/null | \
		grep -F "GNU plotutils")
  GRAPH_EXT := $(if $(GNU_GRAPH),GNUmeta,plot)
  GRAPH_OPT := $(if $(GNU_GRAPH),-O,)
grhist :
	@-grhist $(GRAPH_OPT) < Outputs/emhist > graph.$(GRAPH_EXT)
	@cmpverb grhist graph.$(GRAPH_EXT)
endif


histeq :
	@-histeq < Outputs/ndvihist > histeq1
	@cmpverb -n histeq histeq1
	@-histeq -n 4096 < Outputs/emhist > histeq2
	@cmpverb -n histeq histeq2
	@-histeq -n 4096 -o 0,255 -i 1,4094 < Outputs/emhist > histeq3
	@cmpverb histeq histeq3

cnhist :
	@-cnhist < Outputs/emhist > cnh.txt
	@cmpverb cnhist cnh.txt

cnhist.all :
	$(NOT_TESTING)

lutx :
	@-lutx -i Data/ndvi.ipw < Outputs/histeq1 > ndvi-heq.ipw
	@cmpverb -n lutx ndvi-heq.ipw
	@-lutx -i Data/elev.78 < Outputs/histeq2 > elev-heq.ipw
	@cmpverb -n lutx elev-heq.ipw
	@-lutx -i Data/elev.78 < Outputs/lut12 > elev.78-8
	@cmpverb lutx elev.78-8

mklut :
	@-mklut -i 12 < Outputs/int2 > lut12
	@cmpverb mklut lut12

# ---------------------------------------------------------------------------

classify.Cmds : classify classeq clmask declass
	$(CHECK_CMDS)

classify :
	@-classify -i Data/classvals Data/rad.10.ipw > clrad10.ipw
	@cmpverb classify clrad10.ipw

classeq :
	$(NO_TEST_YET)

clmask :
	@-clmask -c 2 -i Outputs/clrad10.ipw > clradmask.ipw
	@cmpverb clmask clradmask.ipw

declass :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

topocalc.Cmds : deltaz gradient ggradient hor1d horizon ghorizon shade gshade \
		viewcalc viewf gviewf gviewf-mp bmask pmask
	$(CHECK_CMDS)

deltaz :
	$(NO_TEST_YET)

gradient :
	@-gradient Data/elev.78 > grad.78
	@cmpverb -e gradient grad.78

ggradient :
	$(NO_TEST_YET)

hor1d :
	@printf "    Auxiliary command:   $@ (for horizon, viewf)\n"
	@-$(IPW)/aux/horizon/hor1d -a -135       Data/elev.78 > hor.nw
	@cmpverb -n -h hor1d hor.nw
	@-$(IPW)/aux/horizon/hor1d -a -135 -b    Data/elev.78 > hor.nwb
	@cmpverb -n -h hor1d hor.nwb
	@-$(IPW)/aux/horizon/hor1d -a -135 -z 88 Data/elev.78 > hor.nw88
	@cmpverb -h hor1d hor.nw88

horizon :
	@-horizon -a 180 Data/elev.78 > hor.n
	@cmpverb -h horizon hor.n

ghorizon :
	@-ghorizon -a 90.0 -m Data/maskbuf.geo Data/dem.geo >hzs.check
	@cmpverb ghorizon hzs.check

shade :
	@-shade -z 45 -a -135 < Outputs/grad.78  > shade1.78
	@cmpverb -n shade shade1.78
	@-shade -u 0.680436 -a -5.413 < Outputs/grad.78  > shade2.78
	@cmpverb shade shade2.78

gshade :
	$(NO_TEST_YET)

viewcalc :
	@printf "    Auxiliary command:   $@ (for viewf, gviewf)\n"
	@-$(IPW)/aux/viewf/viewcalc -s Outputs/grad.78 -h Data/horizons.78 \
		> skyview-terr.ipw
	@cmpverb -h viewcalc skyview-terr.ipw

viewf :
	@-viewf Data/elev.78  > skyview-terr.ipw
	@cmpverb -h viewf skyview-terr.ipw

gviewf :
	@echo "    Testing gviewf -- please wait."
	@-gviewf -m Data/maskbuf.geo Data/dem.geo >views.out
	@cmpverb -h gviewf views.out

gviewf-mp :
	$(NO_TEST_YET)

bmask :
	$(NO_TEST_YET)

pmask :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

proj.Cmds : mkproj reproj reproj_area transcoord
	$(CHECK_CMDS)

mkproj :
	@-mkproj -p lamaz -l -100,45 > nalambert.defs
	@cmpverb -n $@ nalambert.defs
	@-mkproj -p albers -s 29.5,45.5 -l -96,23 > usalbers.defs
	@cmpverb -n $@ usalbers.defs
	@-mkproj -p utm -n 10 > west.ore.defs.n
	@cmpverb -n $@ west.ore.defs.n
	@-mkproj -p utm -l -121,45 > west.ore.defs.l
	@cmpverb $@ west.ore.defs.l

reproj :
	$(NO_TEST_YET)

reproj_area :
	$(NO_TEST_YET)

transcoord :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

phys.pt.Cmds : albedo glob.alb dewpt satvp rh2vp sling vptr mse pott \
		swe swp refdx zpr
	$(CHECK_CMDS)

albedo :
	$(NO_TEST_YET)

glob.alb :
	$(NO_TEST_YET)

dewpt :
	$(NO_TEST_YET)

satvp :
	$(NO_TEST_YET)

rh2vp :
	$(NO_TEST_YET)

sling :
	$(NO_TEST_YET)

vptr :
	$(NO_TEST_YET)

mse :
	$(NO_TEST_YET)

pott :
	$(NO_TEST_YET)

swe :
	$(NO_TEST_YET)

swp :
	$(NO_TEST_YET)

refdx :
	@-refdx -s ice,water < Data/refwl > refri1
	@cmpverb -n refdx refri1
	@-refdx -s ice,water -u cm < Data/refwl2 > refri1cm
	@cmpverb -n refdx refri1cm
	@-refdx -s water -t 100 < Data/refwl > refri2
	@cmpverb -n refdx refri2
	@-refdx -s ice   -t -55 < Data/refwl > refri3
	@cmpverb -n refdx refri3
	@-refdx -s ice,water -t -7.42 -d < Data/refwl > refdf
	@cmpverb refdx refdf

zpr :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

phys.spat.Cmds : idewpt irh2vp isatvp indvi ipott ialbedo deltaT
	$(CHECK_CMDS)

idewpt :
	@-idewpt -m Data/mask.alb.78 Data/ea.78 > dpt.78
	@cmpverb idewpt dpt.78

irh2vp :
	$(NO_TEST_YET)

isatvp :
	$(NO_TEST_YET)

indvi :
	$(NO_TEST_YET)

ipott :
	$(NO_TEST_YET)

ialbedo :
	$(NO_TEST_YET)

deltaT :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

physmod.pt.Cmds : advec bowvap penmanx pmx ptaylorx ptx trb_Ri trbxfr gsnow \
		  snobal snowmass humidx soiltmp srftmp
	$(CHECK_CMDS)

advec :
	$(NO_TEST_YET)

bowvap :
	$(NO_TEST_YET)

penmanx :
	$(NO_TEST_YET)

pmx :
	$(NO_TEST_YET)

ptaylorx :
	$(NO_TEST_YET)

ptx :
	$(NO_TEST_YET)

trb_Ri :
	$(NO_TEST_YET)

trbxfr :
	$(NO_TEST_YET)

gsnow :
	$(NO_TEST_YET)

snobal :
	@-snobal -z 1142.0 -t 60 -m 0.01 -d 0.25 -s Data/snobal/sn -h Data/snobal/mh -p Data/snobal/pr -i Data/snobal/in -o snobal.out -O normal -c > /dev/null
	@cmpverb -d snobal snobal.out

snowmass :
	$(NO_TEST_YET)

humidx :
	$(NO_TEST_YET)

soiltmp :
	$(NO_TEST_YET)

srftmp :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

physmod.spat.Cmds : ipmx iptx itrbxfr isnobal mkprecip wbal
	$(CHECK_CMDS)

ipmx :
	$(NO_TEST_YET)

iptx :
	$(NO_TEST_YET)

itrbxfr :
	$(NO_TEST_YET)

isnobal :
	$(NO_TEST_YET)

mkprecip :
	$(NO_TEST_YET)

wbal :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

rad.pt.Cmds : solar sunang sunlight twostream thermin trad
	$(CHECK_CMDS)

solar :
	@-solar -d 1990,6,22 -w .58,.68  > col-solar
	@cmpverb solar col-solar

sunang :
	@-sunang -b 34,25 -l -119,54 -t 1990,2,15,12,30 -z 480 -d -f > ucsb-sun
	@cmpverb sunang ucsb-sun

sunlight :
	@-sunlight -d 1990,6,22 -b 37 -l -112,30 -z 480  > col-sunlight
	@cmpverb -n sunlight col-sunlight
	@-sunlight -d 1990,6,22 -b 37 -l -112,30 -z 480 -q 21 > col-sunlightq
	@cmpverb sunlight col-sunlightq

twostream :
	@-twostream -u .5 -t .5 -w .5 -g .5 -r .5 -s .5     > twomw
	@cmpverb -n twostream twomw
	@-twostream -u .5 -t .5 -w .5 -g .5 -r .5 -s .5 -d  > twoed
	@cmpverb twostream twoed

thermin :
	$(NO_TEST_YET)

trad :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

rad.bas.Cmds : elevrad selevrad toporad stoporad toporad.24 topotherm topquad \
		tcloud
	$(CHECK_CMDS)

elevrad :
	@-elevrad -z 100 -t 0.2 -w .85 -g .55 -r .155 -s 150.214 \
	          -u 0.816876 Data/elev.col  > col-bdrad
	@cmpverb elevrad col-bdrad

selevrad :
	$(NO_TEST_YET)

toporad :
	@-toporad Data/top-parms > top-incoming
	@cmpverb -n -e toporad top-incoming
	@-toporad -n Data/top-parms > top-net
	@cmpverb -e toporad top-net

stoporad :
	$(NO_TEST_YET)

toporad.24 :
	$(NO_TEST_YET)

topotherm :
	$(NO_TEST_YET)

topquad :
	@echo "    Testing topquad -- please wait."
	@-topquad -z 200 -t .2 -w .85 -g .55 -r .1244 -x .58,.68 -d 1990,6,22 \
	          -b 48 -l -119,30  Data/topq-parms   > topq-incoming
	@cmpverb topquad topq-incoming

tcloud :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

rad.glb.Cmds : gelevrad gsunlight gsunweights gtoporad gtoporad.24 gtopquad
	$(CHECK_CMDS)

gelevrad :
	$(NO_TEST_YET)

gsunlight :
	$(NO_TEST_YET)

gsunweights :
	$(NO_TEST_YET)

gtoporad :
	$(NO_TEST_YET)

gtoporad.24 :
	$(NO_TEST_YET)

gtopquad :
	$(NO_TEST_YET)

# ---------------------------------------------------------------------------

misc.Cmds : ipw
	$(CHECK_CMDS)

ipw :
	$(NOT_TESTING)
