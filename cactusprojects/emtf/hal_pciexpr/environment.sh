#!/bin/bash

#XDAQ
export LD_LIBRARY_PATH=
export XDAQ_ROOT=/opt/xdaq
export XDAQ_DOCUMENT_ROOT=$XDAQ_ROOT/htdocs
export XDAQ_SETUP_ROOT=$XDAQ_ROOT/share
#export XDAQ_OS=linux
#export XDAQ_PLATFORM=i386
export LD_LIBRARY_PATH=$XDAQ_ROOT/lib:$LD_LIBRARY_PATH

# HAL
export BUILD_HOME=../../
export LD_LIBRARY_PATH=$BUILD_HOME/daq/hal/x86_64_slc6/lib:$LD_LIBRARY_PATH

