# Top Makefile for the HAL library
#
# This makefile works as a normal xdaq makefile in case the XDAQ_ROOT
# environment variable is set.
# If XDAQ_ROOT is not set it is assumed the used wants to compile the
# HAL in stand-alone. Therefore the configuration files for the make
# environment are taken from the hal/config directory and the hal/xcept
# package is compiled locally instead of being taken from the XDAQ tree.
#
# $Id: Makefile,v 1.84 2007/07/04 11:38:51 gutleber Exp $
#

PWD=$(shell pwd)

ifndef XDAQ_ROOT
STANDALONE=1
export STANDALONE
XDAQ_ROOT=$(PWD)
export XDAQ_ROOT
BUILD_HOME=$(PWD)/..
export BUILD_HOME
$(warning ***********************************************************)
$(warning **** HAL will compiled stand-alone no XDAQ environment ****)
$(warning ***********************************************************)
endif

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(XDAQ_ROOT)/config/mfDefs.extern_coretools
include $(XDAQ_ROOT)/config/mfDefs.coretools

Project=daq

Packages= \
          hal/generic \
          hal/utilities \
          hal/busAdapter/dummy \
          hal/busAdapter/caen \
          hal/busAdapter/pci \
          hal/busAdapter/pciexpr 

# Compile SBS busadapter on SLC3 only. It's not yet supported on SLC4

ifeq ($(shell $(XDAQ_ROOT)/config/checkos.sh),slc3)
Package += hal/busAdapter/sbs 
endif


ifdef STANDALONE
include Makefile.in
Packages = hal/xcept \
           hal/generic \
           hal/utilities \
           hal/busAdapter/dummy
ifdef CAENDIR
Packages += hal/busAdapter/caen
endif
ifdef SBSDIR
Packages += hal/busAdapter/sbs
endif
ifdef GENERICPCIDIR
Packages += hal/busAdapter/pci
endif

Project = .
endif

include $(XDAQ_ROOT)/config/Makefile.rules

.PHONY: default
default: build localinstall finalmessage

.PHONY: _all _installall _cleanall _rpmall _installrpmall _changelogall


_all: build localinstall finalmessage


_installall: install  


_cleanall: clean


_rpmall: rpm  


_installrpmall: installrpm

_changelogall: changelog


.PHONY:localinstall
localinstall :
	export INSTALL_PREFIX=$(shell pwd); $(MAKE) -C ./ install

.PHONY:localrpminstall
	export INSTALL_PREFIX=$(shell pwd); $(MAKE) -C ./ rpminstall

.PHONY:localclean
localclean :
	-rm -rf $(XDAQ_PLATFORM)

.PHONY : doc
doc :
	$(MAKE) -C generic/doc doc
	$(MAKE) -C utilities/doc doc
	$(MAKE) -C busAdapter/doc doc


list :
	@echo
	@echo "BUILD_HOME is $(BUILD_HOME)"
	@echo "XDAQ_ROOT  is $(XDAQ_ROOT)"
	@echo "Project    is $(Project)"
	@echo

Makefile.in :
	./configure.pl

finalmessage:
ifdef STANDALONE
	@echo "***************************************************************"
	@echo "**** HAL has been compiled stand-alone no XDAQ environment ****"
	@echo "***************************************************************"
else
	@echo "*************************************************************************************************"
	@echo "   HAL has been compiled in XDAQ environment with XDAQ_ROOT = $(XDAQ_ROOT)"
	@echo "*************************************************************************************************"
endif

include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules
