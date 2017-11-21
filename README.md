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

* Running the application
  - System description in [example904.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/example904.xml)
  and [exampleP5.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/exampleP5.xml)
   is used by the framework to instantiate all of the components at the initialization step
     + **\<uri\>** is the tag identifying which **/dev/utca_sp12\*** device file to use (**\<slot\>** tag has no effect)
  - [runStandalone.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/runStandalone.sh)
   script sets environment and runs [runSubsystemCell.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/runSubsystemCell.sh)
script that generates xml configuration from [common.configure](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/common.configure)
   and runs the XDAQ application on port *3234*
     + add **-g** command line option where the first script launches the second one and you can debug the code in cgdb
  - [emtfTopLevel.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfTopLevel.xml),
    [emtfInfra.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfInfra.xml),
    [emtfAlgo.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfAlgo.xml),
    [emtfRunSettings.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/emtfRunSettings.xml), and 
    [amc13Config.xml](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/amc13Config.xml)
    cell configuration to be used with no database access
     + this allows you not to launch the next *tstore.sh* script and also liberates from typing the TSC and RS keys
     + you'll have to type **file://** in the "Configuration Key* field on the *Run Control* page
  - [tstore.sh](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/tstore.sh) starts an
  independent XDAQ TStore service for accessing the database; also needs a dummy
  [standalone.xhannel](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/standalone.xhannel)
  and [cms\_trg\_r.txt](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/cms_trg_r.txt) read-only DB credential
  - [log4cplus.properties](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/log4cplus.properties)
    is the log4cplus configuration for local logging
    
* System components:
  - [EmtfSystem](https://github.com/kkotov/emtf\_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/EmtfSystem.cpp)
    + registers transitions' listeners in the FSM
    + provides overall health monitoring
  - [EmtfProcessor](https://github.com/kkotov/emtf\_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/EmtfProcessor.cpp)
    + by means of our [HAL counterpart](https://github.com/kkotov/emtf_cell/tree/master/cactusprojects/emtf/pciExprLinuxBusAdapter)
      it reads device's address table and opens the device file 
    + instantiates commands and sequences of commands
    + instantiates port objects
    + registers local monitoring callbacks that writes/reads to/from the hardware
    + logs various parameters in local log files
  - Various \*Port classes
    + monitoring and logs
    + hardware reads/writes

* Commands for configuring the hardware:
  - AMC13
    + comes with the framework and allows to reset/reboot the AMC13 boards similarly to [the command line tool](https://twiki.cern.ch/twiki/bin/view/CMS/EMTFSwatchControlSoftware#How_to_manually_reset_AMC13s)
  - LUTs
    + [WriteVerifyPCLuts.cpp](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/WriteVerifyPCLuts.cpp)
    + [WriteVerifyPtLut.cpp](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/WriteVerifyPtLut.cpp)
  - Algorithmic
    + [AlgoConfig](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/ConfigCommands.cpp#L66-L91)
    + [DoubleMuonTrg](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/ConfigCommands.cpp#L93-L117)
    + [SingleHits](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/SpyFifo.cpp#L55-L74)
  - Guards and checks
    + [Firmware version](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/ConfigCommands.cpp#L14-L64)
  - Logs (does not affect how the system runs)
    + [OnStart and OnStop](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/TransitionCommands.cpp)
    + [Link alignment logs](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/LinksAlignmentReferences.cpp)
  - Timing and other hardware specifics
    + [SetDelaysAndTriggerSource](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/SpyFifo.cpp#L19-L52)
    + [DaqReportWoTrack](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/SpyFifo.cpp#L77-L96)
    + [DAQConfigRegisters](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/DAQConfigRegisters.cpp)
    + [DAQModuleReset](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/Resets.cpp)
    + [Core FPGA reboot](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/Reboot.cpp)
    + [PtLutClockReset](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/Resets.cpp#L116-L157)
    + [ResetCoreLink](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/Resets.cpp#L63-L84)
    + [GthModuleReset](https://github.com/kkotov/emtf_cell/blob/master/cactusprojects/emtf/ts/cell/src/common/Resets.cpp#L91-L113)

* Miscellanea (applicable to p.5 and partially to b.904)
  - Do not use the [DB scripts](https://github.com/kkotov/emtf_cell/tree/master/cactusprojects/emtf/ts/cell/scripts);
  - PCIe driver location: /home/emtfts/driver/
  - EMTF Swatch cell RPMs: /home/emtfts/versions/
  - Configuration (LUTs, Address Tables, delays): /home/emtfts/emtf\_cell\_config
  - Logs with the Link Alignment reference values: /home/emtfts/reference\_values
    + you can turn these numerous files into a single CSV file with
      [this script](https://github.com/kkotov/l1o2o/blob/gh-pages/talks/2017.10.28/scr)
    + this CSV file can be visualized using R + Shiny with
      [*server.R* and *ui.R* scripts](https://github.com/kkotov/l1o2o/tree/gh-pages/talks/2017.10.28)
      scripts in a local directory: *echo "require(shiny); runApp()" > q.R ; Rscript q.R* 

