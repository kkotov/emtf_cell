#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "swatch/core/Factory.hpp"
#include "emtf/ts/cell/Mtf7TTCInterface.hpp"
#include "emtf/ts/cell/Mtf7ReadoutInterface.hpp"
#include "emtf/ts/cell/Mtf7Port.hpp"
#include "emtf/ts/cell/Mtf7AlgoInterface.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "emtf/ts/cell/Mtf7Resets.hpp"
#include "emtf/ts/cell/Mtf7Loopback.hpp"
#include "emtf/ts/cell/Mtf7DAQConfigRegisters.hpp"
#include "emtf/ts/cell/Mtf7SpyFifo.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"
#include "emtf/ts/cell/Mtf7WriteVerifyLuts.hpp"
#include "emtf/ts/cell/TransitionCommands.hpp"
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "swatch/core/StateMachine.hpp"
#include "swatch/core/MetricConditions.hpp"


using namespace std;
using namespace swatch::core;
using namespace swatch::processor;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace log4cplus;

namespace emtf {

SWATCH_REGISTER_CLASS(emtf::Mtf7Processor);

Mtf7Processor::Mtf7Processor(const AbstractStub& aStub) :
    extPllLockStatus(registerMetric<bool>("Ext pll lock status",
                                             NotEqualCondition<bool>(true),
                                             NotEqualCondition<bool>(true))),
    bc0PeriodCounter(registerMetric<int>("Bc0 period counter",
                                           NotEqualCondition<int>(3563),
                                           NotEqualCondition<int>(3563))),
    outputTrackRate(registerMetric<double>("Output track rate (Hz)")),
    brokenLinks(registerMetric<uint16_t>("Number of broken input links",
                                         GreaterThanCondition<uint16_t>(config::brokenLinksErrorProcessor()),
                                         RangeCondition<uint16_t>(config::brokenLinksWarningProcessor(),
                                                                  config::brokenLinksErrorProcessor()))),
    controlFirmwareVersion(registerMetric<string>("Control Firmware Version Timestamp")),
    coreFirmwareVersion(registerMetric<string>   ("Core Firmware Version Timestamp")),
    Processor(aStub),
    addressTableReader(NULL),
    addressTable(NULL),
    driver(NULL),
    generalLogger(Logger::getInstance(config::log4cplusGeneralLogger())),
    rateLogger(Logger::getInstance(config::log4cplusRateLogger()))

{
    const ProcessorStub& stub = getStub();

    addressTableReader = new HAL::PCIAddressTableASCIIReader(stub.addressTable);
    addressTable = new HAL::PCIExprAddressTable("Address Table", *addressTableReader);

    uint32_t mtf7_index = boost::lexical_cast<uint32_t, std::string>(stub.uri);
    driver = new HAL::PCIExprDevice(*addressTable, busAdapter, mtf7_index);

    // Build subcomponents
    registerInterface(new Mtf7TTCInterface(*driver));
    registerInterface(new Mtf7ReadoutInterface(*driver));
    registerInterface(new Mtf7AlgoInterface(*driver));
    registerInterface(new InputPortCollection());
    registerInterface(new OutputPortCollection());

    for(vector<ProcessorPortStub>::const_iterator it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    {
        getInputPorts().addPort(new Mtf7InputPort(it->id, it->number, aStub.id, *driver));
    }

    for(std::vector<ProcessorPortStub>::const_iterator it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    {
        getOutputPorts().addPort(new Mtf7OutputPort(it->id, it->number, *driver));
    }


    Command & cDaqModuleRst = registerCommand<DAQModuleReset>("DAQ Module Reset");
    // Command & cGthModuleReset = registerCommand<ResetGthTransceivers>("GTH Module Reset");
    Command & cSetDaqCfgRegs = registerCommand<Mtf7DAQConfigRegisters>("Set DAQ Config Registers");
    Command & cSetBC0AndDataDelay = registerCommand<Mtf7SetDelaysAndTriggerSource>("Set BC0 and Data Delay");
    Command & cSetSingleHits = registerCommand<Mtf7SetSingleHits>("Enable the single hit algorithm");
    Command & cDaqReportWoTrack = registerCommand<Mtf7DaqReportWoTrack>("Enable the firmware report in DAQ stream");
    // Command & cCheckFWVersion = registerCommand<CheckFWVersion>("Compare the firmware version");
    // Command & cWritePcLuts = registerCommand<WritePcLuts>("Write the PC LUTs to the board");
    Command & cWritePtLuts = registerCommand<WritePtLuts>("Write the pT LUT to the board");
    Command & cVerifyPtLuts = registerCommand<VerifyPtLuts>("Verify the pT LUT to the board");
    Command & cVerifyPcLuts = registerCommand<VerifyPcLuts>("Verify the PC LUTs");
    Command & cVerifyPcLutsVersion = registerCommand<VerifyPcLutsVersion>("Verify the PC LUTs version");
    Command & cOnStart = registerCommand<OnStart>("Executed at the transition from 'Aligned' to 'Running'");

    CommandSequence &cfgSeq = registerSequence("Configure Sequence", cVerifyPcLutsVersion).
                                                                then(cVerifyPcLuts).
                                                                then(cDaqModuleRst).
                                                                then(cSetDaqCfgRegs).
                                                                then(cSetBC0AndDataDelay).
                                                                then(cSetSingleHits).
                                                                then(cDaqReportWoTrack);

    // processor run control state machine
    RunControlFSM &pFSM = getRunControlFSM();
    // pFSM.setup.add(cCheckFWVersion); // TODO: when we enable that we'll need a new DB key
    pFSM.configure.add(cfgSeq);
    // pFSM.align.add(cGthModuleReset);
    pFSM.start.add(cOnStart);

    const string processorMessage("Board " + aStub.id + " (/dev/utca_sp12" + getStub().uri + ") " + "initialized successfully");
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT(processorMessage));
}

Mtf7Processor::~Mtf7Processor()
{
    delete driver;
    delete addressTable;
    delete addressTableReader;
}

uint64_t Mtf7Processor::readFirmwareVersion()
{
    uint32_t controlFirmwareVersion = 0u;
    readControlFirmwareVersion(&controlFirmwareVersion);

    uint32_t coreFirmwareVersion = 0u;
    readCoreFirmwareVersion(&coreFirmwareVersion);

    uint64_t firmwareVersion = (((uint64_t)coreFirmwareVersion) << 32) | controlFirmwareVersion;

    return firmwareVersion;
}

bool Mtf7Processor::readPLLstatus(void)
{
    uint32_t ext_pll_lock = 0u;
    read("ext_pll_lock", ext_pll_lock);

    return ext_pll_lock;
}

int Mtf7Processor::readBC0counter(void)
{
    uint32_t bc0_period_cnt = 0u;
    read("bc0_period_cnt", bc0_period_cnt);

    return bc0_period_cnt;
}

double Mtf7Processor::readTrackRate(void)
{
    uint64_t output_track_counter = 0u;
    read64("output_track_rate", output_track_counter);

    double rate = output_track_counter/0.00327;

    const string rateMsg(getStub().id + " output track rate: " + boost::lexical_cast<string>(rate) + " Hz");

    LOG4CPLUS_WARN(rateLogger, LOG4CPLUS_TEXT(rateMsg));

    return rate;
}

uint16_t Mtf7Processor::countBrokenLinks(void)
{
    uint16_t counter = 0;

    for(auto it=getInputPorts().getPorts().begin(); it!=getInputPorts().getPorts().end(); ++it)
    {
        if( (!(*it)->isMasked()) && (kGood != (*it)->getStatusFlag()) )
        {
            ++counter;
        }
    }

    return counter;
}

string Mtf7Processor::readControlFirmwareVersion(uint32_t *firmwareVersion)
{
    uint32_t ctlFpgaFwSec = 0u;
    read("ctl_fpga_fw_sec", ctlFpgaFwSec);

    uint32_t ctlFpgaFwMin = 0u;
    read("ctl_fpga_fw_min", ctlFpgaFwMin);

    uint32_t ctlFpgaFwHour = 0u;
    read("ctl_fpga_fw_hour", ctlFpgaFwHour);

    uint32_t ctlFpgaFwDay = 0u;
    read("ctl_fpga_fw_day", ctlFpgaFwDay);

    uint32_t ctlFpgaFwMonth = 0u;
    read("ctl_fpga_fw_month", ctlFpgaFwMonth);

    uint32_t ctlFpgaFwYear = 0;
    read("ctl_fpga_fw_year", ctlFpgaFwYear);
    ctlFpgaFwYear += 2000u; // the year is written in the firmware relative to the year 2000

    const string controlFirmwareVersion(        boost::lexical_cast<string>(ctlFpgaFwYear)
                                        + "-" + boost::lexical_cast<string>(ctlFpgaFwMonth)
                                        + "-" + boost::lexical_cast<string>(ctlFpgaFwDay)
                                        + " " + boost::lexical_cast<string>(ctlFpgaFwHour)
                                        + ":" + boost::lexical_cast<string>(ctlFpgaFwMin)
                                        + ":" + boost::lexical_cast<string>(ctlFpgaFwSec));

    if(NULL != firmwareVersion)
    {
        ptime epoch(date(1970,1,1), time_duration(0,0,0));

        ptime controlFirmwareTime(date(ctlFpgaFwYear,ctlFpgaFwMonth, ctlFpgaFwDay),
                                  time_duration(ctlFpgaFwHour, ctlFpgaFwMin, ctlFpgaFwSec));

        *firmwareVersion = (controlFirmwareTime - epoch).total_seconds();
    }

    return controlFirmwareVersion;
}

string Mtf7Processor::readCoreFirmwareVersion(uint32_t *firmwareVersion)
{
    uint64_t coreFpgaFwSec = 0u;
    read64("core_fpga_fw_sec", coreFpgaFwSec);

    uint64_t coreFpgaFwMin = 0u;
    read64("core_fpga_fw_min", coreFpgaFwMin);

    uint64_t coreFpgaFwHour = 0u;
    read64("core_fpga_fw_hour", coreFpgaFwHour);

    uint64_t coreFpgaFwDay = 0u;
    read64("core_fpga_fw_day", coreFpgaFwDay);

    uint64_t coreFpgaFwMonth = 0u;
    read64("core_fpga_fw_month", coreFpgaFwMonth);

    uint64_t coreFpgaFwYear = 0u;
    read64("core_fpga_fw_year", coreFpgaFwYear);
    coreFpgaFwYear += 2000u; // the year is written in the firmware relative to the year 2000

    const string coreFirmwareVersion(        boost::lexical_cast<string>(coreFpgaFwYear)
                                     + "-" + boost::lexical_cast<string>(coreFpgaFwMonth)
                                     + "-" + boost::lexical_cast<string>(coreFpgaFwDay)
                                     + " " + boost::lexical_cast<string>(coreFpgaFwHour)
                                     + ":" + boost::lexical_cast<string>(coreFpgaFwMin)
                                     + ":" + boost::lexical_cast<string>(coreFpgaFwSec));
    if(NULL != firmwareVersion)
    {
        ptime epoch(date(1970,1,1), time_duration(0,0,0));

        ptime coreFirmwareTime(date(coreFpgaFwYear,coreFpgaFwMonth, coreFpgaFwDay),
                                  time_duration(coreFpgaFwHour, coreFpgaFwMin, coreFpgaFwSec));

        *firmwareVersion = (coreFirmwareTime - epoch).total_seconds();
    }

    return coreFirmwareVersion;
}

void Mtf7Processor::retrieveMetricValues()
{
    setMetricValue<uint64_t>(mMetricFirmwareVersion, readFirmwareVersion());
    setMetricValue<string>  (controlFirmwareVersion, readControlFirmwareVersion());
    setMetricValue<string>  (coreFirmwareVersion,    readCoreFirmwareVersion());
    setMetricValue<bool>    (extPllLockStatus,    readPLLstatus());
    setMetricValue<int>     (bc0PeriodCounter,     readBC0counter());
    setMetricValue<double>  (outputTrackRate,      readTrackRate());
    setMetricValue<uint16_t>(brokenLinks,            countBrokenLinks());
}

} // namespace

