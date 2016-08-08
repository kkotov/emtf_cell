# load the project relevant Variables:
ARCH=linux
BSP=x86

HALDIR=../../../../..
include $(HALDIR)/Makefile.in
include ../../../../MakeVars
include $(HALDIR)/makeDefinitions/MakeDefs.$(ARCH)
#include $(TESTERCLASSESDEPFILES)

EXTRA_INCLUDES=-I../../../include

ldpath :
	echo "export LD_LIBRARY_PATH=/usr/local/gcc-alt-3.2.3/lib:../../../../../lib/linux/x86:../../../lib/linux/x86:$(XERCESLIBDIR)" > ldpath
	echo 'echo $$LD_LIBRARY_PATH' >> ldpath

clean :
	-rm *.o
	-rm *~
	-rm *.d
