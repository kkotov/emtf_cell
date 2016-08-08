# $Id: Makefile,v 1.8 2007/06/11 17:30:48 cschwick Exp $

#########################################################################
# XDAQ Components for Distributed Data Acquisition                      #
# Copyright (C) 2000-2004, CERN.			                #
# All rights reserved.                                                  #
# Authors:  L. Orsini					                #
#                                                                       #
# For the licensing terms see LICENSE.		                        #
# For the list of contributors see CREDITS.   			        #
#########################################################################

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.coretools


Project=daq
Package=hal/busAdapter/dummy


Sources = \
	ConfigurationSpaceFileReader.cc \
	VME64xDummyBusAdapter.cc \
	VMEDummyBusAdapter.cc \
	VMEDummyDeviceIdentifier.cc \
        PCIDummyBusAdapter.cc \
        PCIDummyDeviceIdentifier.cc

DynamicLibrary= DummyBusAdapters

StaticLibrary=

IncludeDirs = \
        $(BUILD_HOME)/$(Project)/hal/generic/include

ifdef STANDALONE
Project = .
IncludeDirs += $(BUILD_HOME)/$(Project)/hal/xcept/include
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
