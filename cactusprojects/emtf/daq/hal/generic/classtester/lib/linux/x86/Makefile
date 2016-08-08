# this Makefile in general is called by ../Makefile

# some general definitions:
HALDIR := ../../../../../
ARCH=linux
BSP=x86
include ../../../../../Makefile.in
include $(HALDIR)/generic/MakeVars
include ../../../../../makeDefinitions/MakeDefs.$(ARCH)

SOURCEOBJECTS     = $(TESTERCLASSESOBJECTFILES:%=../../../src/$(ARCH)/$(BSP)/%)

%.so : $(SOURCEOBJECTS)
	$(CC) -shared -o $@ $(SOURCEOBJECTS)
