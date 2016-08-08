# this Makefile should be called by the main makefile in the ../src 
# subdirectory

# include project relevant variables

include ../../../../makeDefinitions/MakeDefs.$(ARCH)

SOURCEPATH = ../../../src/$(ARCH)/$(BSP)

SOURCEOBJECTS  = $(OBJECTFILES:%=$(SOURCEPATH)/%)
SOURCEPICS     = $(PICFILES:%=$(SOURCEPATH)/%)

libVxWorksMv2304BusAdapter : $(SOURCEOBJECTS)
	-rm $@
	$(AR) $(ARFLAGS) $@ $(SOURCEPATH)/VMEVxWorksMv2304BusAdapter.o
	$(RANLIB) $@
	strip $@

%.a : $(SOURCEOBJECTS)
	-rm $@
	$(AR) $(ARFLAGS) $@ $(SOURCEOBJECTS)
	$(RANLIB) $@
	strip $@

