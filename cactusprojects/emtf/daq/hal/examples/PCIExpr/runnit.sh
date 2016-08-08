#!/bin/sh
export LD_LIBRARY_PATH=/build/trunk/daq/hal/examples/PCIExpr/../../x86_64_slc6/lib:/opt/xdaq//lib:/lib
echo $LD_LIBRARY_PATH
bin/linux/x86_64_slc6/PCIExprExample.exe
