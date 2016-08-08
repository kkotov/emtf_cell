# load the project relevant Variables:
ARCH=linux
BSP=x86

HALDIR=../../../../..
include $(HALDIR)/Makefile.in
include $(HALDIR)/generic/MakeVars
include $(HALDIR)/makeDefinitions/MakeDefs.$(ARCH)
include $(TESTERCLASSESDEPFILES)

EXTRA_INCLUDES=-I../../../include

testlib : $(TESTERCLASSESOBJECTFILES)
	$(MAKE) -C ../../../lib/linux/x86 libTester.so

ldpath :
	echo "export LD_LIBRARY_PATH=../../../../../lib/linux/x86:../../../lib/linux/x86:$(XERCESLIBDIR):../../../../../../../daq/xcept/lib/linux/x86" > ldpath
	echo 'echo $$LD_LIBRARY_PATH' >> ldpath


clean :
	-rm *.o
	-rm *~
	-rm *.d



