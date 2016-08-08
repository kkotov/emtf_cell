# $Id: Makefile,v 1.2 2007/04/10 08:50:58 cschwick Exp $

#########################################################################
# XDAQ Components for Distributed Data Acquisition                      #
# Copyright (C) 2000-2004, CERN.			                #
# All rights reserved.                                                  #
# Authors: J. Gutleber and L. Orsini					#
#                                                                       #
# For the licensing terms see LICENSE.		                        #
# For the list of contributors see CREDITS.   			        #
#########################################################################

#
# Makefile if TriDAS/config directory
#
BUILD_HOME:=$(shell pwd)/..

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

Project=daq
Package=../config
PackageName=build

PACKAGE_VER_MAJOR=3
PACKAGE_VER_MINOR=9
PACKAGE_VER_PATCH=0

Summary=Build infrastructure (generic makefiles)

Description=Build infrastructure (generic makefiles)

Authors=Johannes Gutleber & Luciano Orsini

Link=http://xdaqwiki.cern.ch

include $(XDAQ_ROOT)/config/mfBuildRPM.rules


