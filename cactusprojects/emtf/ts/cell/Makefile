#########################################################################
# Trigger Supervisor Control Cell skeleton                              #
# Ildefons Magrans                                                      #
#########################################################################
BUILD_HOME:=$(shell pwd)/../../../..

include $(BUILD_HOME)/config/Makefile.macros

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

Project=cactusprojects
Package=emtf/ts/cell

export PACKAGE_VER_MAJOR=0
export PACKAGE_VER_MINOR=7
export PACKAGE_VER_PATCH=0
export PACKAGE_RELEASE=0

Sources=\
	version.cc \
	Cell.cc \
    Mtf7System.cpp \
	Mtf7Processor.cpp \
	Mtf7TTCInterface.cpp \
	Mtf7ReadoutInterface.cpp \
	Mtf7Port.cpp \
	Mtf7AlgoInterface.cpp \
    Mtf7Common.cpp \
	Mtf7Resets.cpp \
	Mtf7Reboot.cpp \
	Mtf7Loopback.cpp \
	Mtf7DAQConfigRegisters.cpp \
	Mtf7SpyFifo.cpp \
	Mtf7LinksAlignmentReferences.cpp \
	Mtf7ConfigCommands.cpp \
	Mtf7WriteVerifyLuts.cpp \
	TransitionCommands.cpp

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(CACTUS_ROOT)/include \
	$(TRIGGER_EMTF_TS_CELL_INCLUDE_PREFIX) \
	$(TRIGGER_EMTF_INCLUDE_PREFIX) \
	$(XDAQ_ROOT)/include/hal \

UserCFlags =
UserCCFlags = -g -ggdb -O0 -std=c++0x

UserDynamicLinkFlags=

DependentLibraryDirs = \
	$(XDAQ_ROOT)/lib \
	$(CACTUS_ROOT)/lib \
	$(TRIGGER_EMTF_TS_CELL_LIB_PREFIX) \
	$(TRIGGER_EMTF_SPBASEINTERACTIONS_LIB_PREFIX) \
	$(TRIGGER_EMTF_SPFUNCTIONS_LIB_PREFIX) \
	$(TRIGGER_EMTF_SPLUTSANDFIRMWARE_LIB_PREFIX) \

DependentLibraries = tsframework \
                     swatchcellframework \
                     cactus_uhal_uhal \
                     cactus_swatch_amc13 \
                     PCIExprLinuxBusAdapter \
                     log4cplus \

UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing
#

Libraries =

#
# Compile the source files and create a shared library
#
DynamicLibrary= emtfcell

StaticLibrary=

include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules

