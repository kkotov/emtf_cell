## Short description

The [EMTF](http://iopscience.iop.org/1748-0221/8/12/C12034) is one of the custom-built
electronic systems of the CMS Level-1 Trigger (L1T). By means of pattern recognition EMTF
combines multiple track's segments produced by a particle in sensitive regions of the
detector and evaluates track's properties. The most important property, the particle's
momentum (or transverse momentum), is evaluated from track's bending in the inhomogeneous
magnetic field.

The EMTF system is controlled and monitored via PCI express (PCIe) connection to a regular
PC running the EMTF cell instance. EMTF cell is a system-specific implementation of 
[SWATCH](http://cactus.web.cern.ch/cactus/tsdocs/index.html) cell. EMTF cell provides an
expert-level web interface to the system and in the same time allows the central SWATCH
framework to orchestrate all systems of the L1T coherently. Current repository contains
the EMTF cell implementation. In the list below I summarize the components of the EMTF
swatch application.

* Miscellaneous
  - system description in [example904.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/example904.xml)
  and [exampleP5.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/exampleP5.xml)
   is used by the framework to instantiate all of the components at the initialization step
     + **<uri>** is the tag identifying which **/dev/utca_sp12** device file to use (**<slot>** tag has no effect)
  - [runStandalone.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/runStandalone.sh)
   script sets environment and runs [runSubsystemCell.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/runSubsystemCell.sh)
script that generates xml configuration from [common.configure](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/common.configure)
   and runs the XDAQ application on port 3234
  - [emtfTopLevel.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfTopLevel.xml),
    [emtfInfra.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfInfra.xml),
    [emtfAlgo.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfAlgo.xml),
    [emtfRunSettings.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfRunSettings.xml), and 
    [amc13Config.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/amc13Config.xml)
    cell configuration to be used with no database access
  - [tstore.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/tstore.sh) starts an
  independent XDAQ TStore service for accessing the database; also needs a dummy
  [standalone.xhannel](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/standalone.xhannel)
  and [cms\_trg\_r.txt](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/cms_trg_r.txt) read-only DB credentials
  - [log4cplus.properties](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/log4cplus.properties)
    is the log4cplus configuration for local logging
    
* System components:
  - [EmtfSystem](https://github.com/kkotov/emtf\_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/EmtfSystem.cpp)
    + registers transitions' listeners in the FSM
    + provides overall health monitoring
  - [EmtfProcessor](https://github.com/kkotov/emtf\_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/EmtfProcessor.cpp)
    + by means of our [https://github.com/kkotov/emtf_cell/tree/master/cactusprojects/emtf/pciExprLinuxBusAdapter](HAL counterpart)
      opens device file and address table
    + instantiates commands and sequences of commands
    + instantiates port objects
    + registers local monitoring callbacks that writes/reads to/from the hardware
    + logs various parameters in local log files
  - Various \*Port classes
    + monitoring and logs
    + read/write register calls

