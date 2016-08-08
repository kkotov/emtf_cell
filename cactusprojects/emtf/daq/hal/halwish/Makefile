HALDIR = $(shell /bin/pwd)/..

include ../makeDefinitions/MakeDefs.linux
include ../Makefile.in

XERCES := $(XERCESLIBDIR)/lib$(XERCESLIBRARY).so
VMEINC := -I$(NATIONALDIR)/include -I$(SBSDIR)/$(SBSVERSION)/include
VMENILIB := -L$(NATIONALDIR)/lib -lniini32 -lcfgeng -lnivxi -ldmamgr $(HALDIR)/lib/linux/x86/libMXI2LinuxX86BusAdapter.so
VMESBSLIB :=  $(HALDIR)/lib/linux/x86/libSBSLinuxX86BusAdapter.so \
	      $(SBSDIR)/$(SBSVERSION)/lib/libbtp.a 
PCIINC := -I $(I2OCOREDIR)/generic_pci_access/include \
          -I $(I2OCOREDIR)/xdaq-shell/include \
          -I $(I2OCOREDIR)/i2o/include
PCILIB := $(HALDIR)/lib/linux/x86/libPCIi386BusAdapter.so \
          $(I2OCOREDIR)/generic_pci_access/libgeneric_pci.a \
          $(I2OCOREDIR)/xdaq-shell/libxdaq-shell.a

.PHONY = clean

halwish-sbs : halAppInt-sbs.o SBSCard.o  Makefile
	$(CXX) -Wall -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux halAppInt-sbs.o SBSCard.o \
        $(HALDIR)/lib/linux/x86/libGenericHAL.so \
        $(XERCES) \
        $(VMESBSLIB) \
        $(PCILIB) \
        /usr/lib/libtk8.3.so \
        /usr/lib/libtcl8.3.so \
        -lpthread \
        -o halwish-sbs

halwish-ni : halAppInt-ni.o NICard.o  Makefile
	$(CXX) -Wall -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux halAppInt-ni.o NICard.o  \
        $(HALDIR)/lib/linux/x86/libGenericHAL.so \
        $(XERCES) \
        $(VMENILIB) \
        $(PCILIB) \
        /usr/lib/libtk8.3.so \
        /usr/lib/libtcl8.3.so \
        -lpthread \
        -o halwish-nivxi

halwish-pci : halAppInt-pci.o PCICard.o Makefile
	$(CXX) -Wall -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux halAppInt-pci.o PCICard.o \
        $(HALDIR)/lib/linux/x86/libGenericHAL.so \
        $(XERCES) \
        $(PCILIB) \
        /usr/lib/libtk8.3.so \
        /usr/lib/libtcl8.3.so \
        -lpthread \
        -o halwish-pci

SBSCard.o : SBSCard.cc Makefile
	$(CXX) -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c SBSCard.cc \
        -DBT1003 \
        $(VMEINC) \
        -I$(HALDIR)/generic/include \
        -I$(HALDIR)/generic/include/linux \
        -I$(HALDIR)/busAdapter/include

NICard.o : NICard.cc Makefile
	$(CXX) -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c NICard.cc \
        -DVXILINUX \
        $(VMEINC) \
        -I$(HALDIR)/generic/include \
        -I$(HALDIR)/generic/include/linux \
        -I$(HALDIR)/busAdapter/include

PCICard.o : PCICard.cc PCICard.h Makefile
	    $(CXX) -g -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c PCICard.cc \
            $(PCIINC) \
            -I$(HALDIR)/generic/include \
            -I$(HALDIR)/generic/include/linux \
            -I$(HALDIR)/busAdapter/include


halAppInt-pci.o : halAppInt-pci.cc Makefile
	$(CXX) -g -D__VOID -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c halAppInt-pci.cc \
        -DBT1003 -DVXILINUX \
        $(VMEINC) \
        $(PCIINC) \
        -I$(HALDIR)/generic/include \
        -I$(HALDIR)/generic/include/linux \
        -I$(HALDIR)/busAdapter/include

halAppInt-sbs.o : halAppInt-sbs.cc Makefile
	$(CXX) -g -D__VOID -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c halAppInt-sbs.cc \
        -DBT1003 -DVXILINUX \
        $(VMEINC) \
        $(PCIINC) \
        -I$(HALDIR)/generic/include \
        -I$(HALDIR)/generic/include/linux \
        -I$(HALDIR)/busAdapter/include

halAppInt-ni.o : halAppInt-ni.cc Makefile
	$(CXX) -g -D__VOID -D_G_HAVE_BOOL -DLITTLE_ENDIAN__ -Dlinux -c halAppInt-ni.cc \
        -DBT1003 -DVXILINUX \
        $(VMEINC) \
        $(PCIINC) \
        -I$(HALDIR)/generic/include \
        -I$(HALDIR)/generic/include/linux \
        -I$(HALDIR)/busAdapter/include

clean :
	-rm *.o
	-rm halwish
