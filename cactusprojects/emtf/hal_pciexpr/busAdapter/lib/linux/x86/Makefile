# this Makefile in general is called by the Makefile in the ../src 

# some general definitions:
HALDIR = ../../../..
ARCH=linux
BSP=x86

# include project relevant variables
include ../../../../Makefile.in
include ../../../../makeDefinitions/MakeDefs.$(ARCH)

SOURCEPATH = ../../../src/$(ARCH)/$(BSP)

SOURCEOBJECTS  = $(OBJECTFILES:%=$(SOURCEPATH)/%)

libSBSLinuxX86BusAdapter.so : $(SOURCEPATH)/SBS620x86LinuxDeviceIdentifier.o \
                              $(SOURCEPATH)/SBS620x86LinuxBusAdapter.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/SBS620x86LinuxDeviceIdentifier.o \
            $(SOURCEPATH)/SBS620x86LinuxBusAdapter.o

libCAENLinuxBusAdapter.so : $(SOURCEPATH)/CAENLinuxBusAdapter.o \
                            $(SOURCEPATH)/CAENDeviceIdentifier.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/$(SOURCEPATH)/CAENDeviceIdentifier.o\
            $(SOURCEPATH)/$(SOURCEPATH)/CAENLinuxBusAdapter.o

libMXI2LinuxX86BusAdapter.so : $(SOURCEPATH)/MXI2x86LinuxBusAdapter.o \
                              $(SOURCEPATH)/MXI2x86LinuxDeviceIdentifier.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/MXI2x86LinuxBusAdapter.o \
            $(SOURCEPATH)/MXI2x86LinuxDeviceIdentifier.o

libVMEDummyBusAdapter.so : $(SOURCEPATH)/VMEDummyDeviceIdentifier.o \
                           $(SOURCEPATH)/VMEDummyBusAdapter.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/VMEDummyDeviceIdentifier.o \
            $(SOURCEPATH)/VMEDummyBusAdapter.o

libVME64xDummyBusAdapter.so : $(SOURCEPATH)/VMEDummyDeviceIdentifier.o \
                              $(SOURCEPATH)/VME64xDummyBusAdapter.o \
                              $(SOURCEPATH)/ConfigurationSpaceFileReader.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/VMEDummyDeviceIdentifier.o \
            $(SOURCEPATH)/VME64xDummyBusAdapter.o \
            $(SOURCEPATH)/ConfigurationSpaceFileReader.o

libPCIDummyBusAdapter.so : $(SOURCEPATH)/PCIDummyDeviceIdentifier.o \
                           $(SOURCEPATH)/PCIDummyBusAdapter.o
	$(CC) -shared -o $@ \
            $(SOURCEPATH)/PCIDummyDeviceIdentifier.o \
            $(SOURCEPATH)/PCIDummyBusAdapter.o

libPCIi386BusAdapter.so : $(SOURCEPATH)/PCIi386DeviceIdentifier.o \
                          $(SOURCEPATH)/PCIi386BusAdapter.o
	$(CC) -shared -o $@ \
             $(SOURCEPATH)/PCIi386DeviceIdentifier.o \
             $(SOURCEPATH)/PCIi386BusAdapter.o

libPCILinuxBusAdapter.so : $(SOURCEPATH)/PCILinuxDeviceIdentifier.o \
                            $(SOURCEPATH)/PCILinuxBusAdapter.o
	$(CC) -shared -o $@ \
             $(SOURCEPATH)/PCILinuxDeviceIdentifier.o \
             $(SOURCEPATH)/PCILinuxBusAdapter.o

