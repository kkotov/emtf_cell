# $Id: Makefile,v 1.10 2007/11/05 16:26:02 lorsini Exp $

#########################################################################
# XDAQ Components for Distributed Data Acquisition                      #
# Copyright (C) 2000-2004, CERN.			                            #
# All rights reserved.                                                  #
# Authors:  L. Orsini					                                #
#                                                                       #
# For the licensing terms see LICENSE.		                            #
# For the list of contributors see CREDITS.   			                #
#########################################################################

XDAQ_ROOT=/opt/xdaq

BUILD_HOME:=$(shell pwd)/../../../

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.hardware_worksuite
include $(XDAQ_ROOT)/config/mfDefs.coretools


-include $(BUILD_HOME)/config/Makefile.macros
include $(BUILD_HOME)/cactusprojects/emtf/config/mfDefs.triggeremtf


Project=cactusprojects
Package=emtf/pciExprLinuxBusAdapter

Sources = \
	common/PCIExprLinuxBusAdapter.cc \
	common/PCIExprLinuxDeviceIdentifier.cc \
	common/PCIExprDevice.cc

DynamicLibrary=PCIExprLinuxBusAdapter

StaticLibrary=

#$(XDAQ_ROOT)/include/hal/generic/include

IncludeDirs=\
	/nfshome0/emtfts/cactus_trunk/cactusprojects/emtf/daq/hal/generic/include \
	$(BUILD_HOME)/cactusprojects/emtf/pciExprLinuxBusAdapter/include/emtf/pciExprLinuxBusAdapter/


ifdef STANDALONE
Project = emtf
IncludeDirs += $(XDAQ_ROOT)/hal/xcept/include
else
Sources += version.cc
IncludeDirs += \
	$(XCEPT_INCLUDE_PREFIX) \
	$(CONFIG_INCLUDE_PREFIX)
endif

UserSourcePath = \
	src

LibraryDirs=

DependentLibraries=GenericHAL

UserCFlags =
UserCCFlags = -g -DLINUX
UserDynamicLinkFlags= -Xlinker -rpath -Xlinker /nfshome0/emtfts/cactus_trunk/cactusprojects/emtf/daq/hal/generic/lib/linux/x86_64_slc6/
UserStaticLinkFlags=
UserExecutableLinkFlags=

# These libraries can be platform specific and
# potentially need conditional processing

ExternalObjects=

# Compile the source files and create a shared library

Executables=

# Local Makefile.rules is a copy of TriDAS/config/Makefile.rules plus a
# modification to use the Libraries tag when building a shared library
include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules

