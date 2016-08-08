# load the project relevant Variables:

ARCH=vxworks
BSP=mv2304

include ../../../MakeVars
include ../../../../makeDefinitions/MakeDefs.$(ARCH)

include  $(DEPFILES)

export

cleano :
	-rm *.o

clean : cleano
	-rm *~
	-rm *.d

force : cleano all

