#                         -*-Makefile-*-
# $Id: Makefile,v 1.2 2007/04/10 08:05:54 cschwick Exp $
PWD=$(shell pwd)

BUILD_HOME=$(PWD)/../..

ifndef XDAQ_ROOT
STANDALONE=1
export STANDALONE
XDAQ_ROOT=/opt/xdaq
BUILD_HOME=$(PWD)/../..
#include $(PWD)/../../Makefile.in
endif

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.coretools
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

.PHONY:runnit.sh
runnit.sh : _all
	echo > runnit.sh "#!/bin/sh"
	echo >> runnit.sh "export LD_LIBRARY_PATH=$(PWD)/../../lib/$(XDAQ_OS)/$(XDAQ_PLATFORM):$(XDAQ_ROOT)/lib:$(XERCESDIR)/lib"
	echo >> runnit.sh 'echo $$LD_LIBRARY_PATH'
	echo >> runnit.sh "bin/$(XDAQ_OS)/$(XDAQ_PLATFORM)/PCIExprExample.exe"
	chmod +x runnit.sh

Project=.
#Project=
Package=examples/PCIExpr
ifdef STANDALONE
Project = .
endif

Sources =

DynamicLibrary=
StaticLibrary=

IncludeDirs = \
        $(BUILD_HOME)/include/ \
        $(BUILD_HOME)/../daq/hal/generic/include/ \
        $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/include \
        $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/include/$(XDAQ_OS) \
        $(XDAQ_ROOT)/include \

LibraryDirs = $(BUILD_HOME)/../daq/hal/generic/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
              $(BUILD_HOME)/$(Project)/$(XDAQ_PLATFORM)/lib \
              $(XDAQ_ROOT)/lib \
              $(BUILD_HOME)/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)


#Libraries = xcept toolbox asyncresolv uuid log4cplus GenericHAL HALUtilities DummyBusAdapters PCIExprLinuxBusAdapterStandAlone xpci
Libraries = xcept toolbox asyncresolv uuid log4cplus GenericHAL HALUtilities PCIExprLinuxBusAdapter GenericHAL xpci

# these two libraries are needed to compile with the standalone version of PCIExpr bus adaptor
Libraries += config xerces-c

ifndef STANDALONE
Libraries += config xerces-c
IncludeDirs += $(XCEPT_INCLUDE_PREFIX)
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
UserExecutableLinkFlags =
# These libraries can be platform specific and
# potentially need conditional processing

ExternalObjects =

# Compile the source files and create a shared library

Executables=PCIExprExample.cc

# Local Makefile.rules is a copy of TriDAS/config/Makefile.rules plus a
# modification to use the Libraries tag when building a shared library
include $(XDAQ_ROOT)/config/Makefile.rules
