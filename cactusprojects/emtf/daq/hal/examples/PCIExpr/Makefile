#                         -*-Makefile-*-
# $Id: Makefile,v 1.2 2007/04/10 08:05:54 cschwick Exp $
PWD=$(shell pwd)

ifndef XDAQ_ROOT
STANDALONE=1
export STANDALONE
XDAQ_ROOT=$(PWD)/../..
BUILD_HOME=$(PWD)/../..
include $(PWD)/../../Makefile.in
endif

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.coretools
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

.PHONY:runnit.sh
runnit.sh : _all
	echo > runnit.sh "#!/bin/sh"
	echo >> runnit.sh "export LD_LIBRARY_PATH=$(PWD)/../../$(XDAQ_PLATFORM)/lib:$(XDAQ_ROOT)/lib:$(XERCESDIR)/lib"
	echo >> runnit.sh 'echo $$LD_LIBRARY_PATH'
	echo >> runnit.sh "bin/$(XDAQ_OS)/$(XDAQ_PLATFORM)/PCIExprExample.exe"
	chmod +x runnit.sh

Project=daq/hal
#Project=
Package=examples/PCIExpr
ifdef STANDALONE
Project = .
endif

Sources =

DynamicLibrary=
StaticLibrary=

IncludeDirs = \
        $(BUILD_HOME)/$(Project)/generic/include \
        $(BUILD_HOME)/daq/pciExprLinuxBusAdapter/include \
        $(XDAQ_ROOT)/include

LibraryDirs = $(BUILD_HOME)/$(Project)/generic/lib/linux/$(XDAQ_PLATFORM) \
              $(BUILD_HOME)/daq/pciExprLinuxBusAdapter/lib/linux/$(XDAQ_PLATFORM) \
              $(XDAQ_ROOT)/lib

Libraries = xcept toolbox asyncresolv uuid log4cplus GenericHAL HALUtilities DummyBusAdapters PCIExprLinuxBusAdapter xpci

ifndef STANDALONE
Libraries += config xerces-c
IncludeDIrs += $(XCEPT_INCLUDE_PREFIX)
endif


# implies STANDALONE
ifdef XERCESDIR
   IncludeDirs += $(XERCESDIR)/include
   LibraryDirs += $(XERCESDIR)/lib
   Libraries   += xerces-c
endif

DependentLibraryDirs = 

DependentLibraries = 

UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags = -Xlinker -rpath -Xlinker /build/trunk/daq/pciExprLinuxBusAdapter/lib/linux/x86_64_slc6/ \
                          -Xlinker -rpath -Xlinker /build/trunk/daq/hal/generic/lib/linux/x86_64_slc6/

# These libraries can be platform specific and
# potentially need conditional processing

ExternalObjects =

# Compile the source files and create a shared library

Executables=PCIExprExample.cc

# Local Makefile.rules is a copy of TriDAS/config/Makefile.rules plus a
# modification to use the Libraries tag when building a shared library
include $(XDAQ_ROOT)/config/Makefile.rules
