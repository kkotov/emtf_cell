##
#
# Sub-Package level makefile
#
##

BUILD_HOME?=$(shell pwd)/../../../..

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.coretools

# Package to be built

Project=daq/hal
Package=examples/XDAQ/Trigger

Sources = Trigger.cc \
          TTCviCard.cc \
          version.cc

IncludeDirs = \
        $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/include \
        $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/include/$(XDAQ_OS) \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \

UserCFlags =
UserCCFlags = -DBT1003 -DLINUX
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

DependentLibraryDirs = $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/lib
DependentLibraries = DummyBusAdapters GenericHAL 

DynamicLibrary= Trigger
StaticLibrary=
Executables=
TestExecutables=
TestLibraries=

include $(XDAQ_ROOT)/config/Makefile.rules
