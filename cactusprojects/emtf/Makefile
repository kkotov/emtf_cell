export XDAQ_ROOT=/opt/xdaq

BUILD_HOME:=$(shell pwd)/../../

-include $(BUILD_HOME)/config/Makefile.macros

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

export PACKAGE_VER_MAJOR=0
export PACKAGE_VER_MINOR=5
export PACKAGE_VER_PATCH=1
export PACKAGE_RELEASE=1

Project=cactusprojects/emtf

Packages= \
        daq/hal \
        pciExprLinuxBusAdapter \
        ts/cell \


include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules

