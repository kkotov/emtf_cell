Introduction:
=============
This is a small XDAQ application which controls the TTCvi.
This application has been used in the XDAQ environment of 
the DAQ-Column. It is used to set and readout some paramenters
of the TTCvi module (VME). 

!!! IMPORTANT !!!  BusAdapters:
================================
The example is running with the VMEDummyBusAdapter. Therefore
everybody can compile the program and play with it (if a running
XDAQ environment is set up). The Makefile for old XDAQ environments
(version V1_2 and earlier) is in the directory

???/hal/examples/XDAQTTCvi/Trigger/src/linux/x86 

The Makefile for V1_3 and new is in 

???/hal/examples/XDAQTTCvi/Trigger

The Makefiles have examined and possibly some paths must be adjusted
in order to run the example.

Also the xml configuration file must be edited so that the paths therein
are correct.

Be aware that your LD_LIBRARY_PATH must contain the paths to the libraries
of the HAL in 

???/hal/lib/linux/x86

In order to use different busAdapters you must edit the source and the 
Makefile.

ATTENTION: 
In case you want to work with the busAdapter for the MXI2
interface of National Instruments, be aware that the library of 
National is a SHARED LIBRARY. (Normally in 
/usr/local/nivxi/lib/libnivxi.so.) This means that the path to 
this library must be set in your environment variable LD_LIBRARY_PATH.
Otherwise when the Trigger application is loaded into the xdaq.exe 
excecutable (when you configure the excecutable) xdaq will hang.


