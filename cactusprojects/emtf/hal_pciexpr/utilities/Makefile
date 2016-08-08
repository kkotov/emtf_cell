# $Id: Makefile,v 1.8 2007/06/11 17:30:50 cschwick Exp $

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.coretools


Project=daq
Package=hal/utilities
ifdef STANDALONE
Project = .
XCEPT_INCLUDE_PREFIX=../xcept/include
endif

Sources = \
          PersistentCommandSequencer.cc \
          TestMaster.cc \
          ConfigROMDumper.cc \
          ConfigCSRDumper.cc

DynamicLibrary= HALUtilities
StaticLibrary=

IncludeDirs = \
        $(BUILD_HOME)/$(Project)/hal/generic/include


ifdef STANDALONE
IncludeDirs +=  $(BUILD_HOME)/$(Project)/hal/xcept/include
else
Sources += version.cc
IncludeDirs += \
	$(XCEPT_INCLUDE_PREFIX) \
	$(XERCES_INCLUDE_PREFIX) \
	$(CONFIG_INCLUDE_PREFIX)
endif

LibraryDirs = 


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

Executables=

# Local Makefile.rules is a copy of TriDAS/config/Makefile.rules plus a
# modification to use the Libraries tag when building a shared library
include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules
