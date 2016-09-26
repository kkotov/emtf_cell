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
#include "emtf/ts/cell/Mtf7Reboot.hpp"
#include "emtf/ts/cell/Mtf7Loopback.hpp"
#include "emtf/ts/cell/Mtf7DAQConfigRegisters.hpp"
#include "emtf/ts/cell/Mtf7SpyFifo.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"
#include "emtf/ts/cell/Mtf7WriteVerifyLuts.hpp"

#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "swatch/core/StateMachine.hpp"
#include "swatch/core/MetricConditions.hpp"


using namespace std;
using namespace swatch::core;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace log4cplus;

namespace emtf {

SWATCH_REGISTER_CLASS(emtf::Mtf7Processor);

Mtf7Processor::Mtf7Processor(const swatch::core::AbstractStub& aStub) :
    ext_pll_lock_status(registerMetric<bool>     ("extPllLockStatus", swatch::core::NotEqualCondition<bool>(true), swatch::core::NotEqualCondition<bool>(true))),
    bc0_period_counter (registerMetric<int>      ("bc0PeriodCounter", swatch::core::NotEqualCondition<int>(3563),  swatch::core::NotEqualCondition<int>(3563))),
    output_track_rate  (registerMetric<double>   ("outputTrackRateInHz")),
    controlFirmwareVersion(registerMetric<string>("controlFwVersionTimestamp")),
    coreFirmwareVersion(registerMetric<string>   ("coreFwaVersionTimestamp")),
    swatch::processor::Processor(aStub),
    addressTableReader(NULL),
    addressTable(NULL),
    driver_(NULL),
    rateLogger(Logger::getInstance(config::log4cplusRateLogger()))
{
    const swatch::processor::ProcessorStub& stub = getStub();

    addressTableReader = new HAL::PCIAddressTableASCIIReader(stub.addressTable);
    addressTable = new HAL::PCIExprAddressTable("Address Table", *addressTableReader);

    uint32_t mtf7_index = boost::lexical_cast<uint32_t, std::string>(stub.uri);
    driver_ = new HAL::PCIExprDevice(*addressTable, busAdapter, mtf7_index);

    // Build subcomponents
    registerInterface(new Mtf7TTCInterface(*driver_));
    registerInterface(new Mtf7ReadoutInterface(*driver_));
    registerInterface(new Mtf7AlgoInterface(*driver_));
    registerInterface(new swatch::processor::InputPortCollection());
    registerInterface(new swatch::processor::OutputPortCollection());

    for(vector<swatch::processor::ProcessorPortStub>::const_iterator it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    {
        getInputPorts().addPort(new Mtf7InputPort(it->id, it->number, aStub.id, *driver_));
    }

    for(std::vector<swatch::processor::ProcessorPortStub>::const_iterator it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    {
        getOutputPorts().addPort(new Mtf7OutputPort(it->id, it->number, *driver_));
    }


    Command & cDaqModuleRst = registerCommand<DAQModuleReset>("DAQ Module Reset");
    // Command & cGthModuleReset = registerCommand<ResetGthTransceivers>("GTH Module Reset");
    Command & cSetDaqCfgRegs = registerCommand<Mtf7DAQConfigRegisters>("Set DAQ Config Registers");
    Command & cSetBC0AndDataDelay = registerCommand<Mtf7SetDelaysAndTriggerSource>("Set BC0 and Data Delay");
    Command & cSetSingleHits = registerCommand<Mtf7SetSingleHits>("Enable the single hit algorithm");
    Command & cDaqReportWoTrack = registerCommand<Mtf7DaqReportWoTrack>("Enable the firmware report in DAQ stream");
    // Command & cCheckFWVersion = registerCommand<CheckFWVersion>("Compare the firmware version");
    // Command & cWritePcLuts = registerCommand<WritePcLuts>("Write the PC LUTs to the board");
    Command & cWritePtLut = registerCommand<WritePtLut>("Write the pT LUT to the board");
    Command & cVerifyPtLut = registerCommand<VerifyPtLut>("Verify the pT LUT to the board");
    Command & cVerifyPcLuts = registerCommand<VerifyPcLuts>("Verify the PC LUTs");
    Command & cVerifyPcLutsVersion = registerCommand<VerifyPcLutsVersion>("Verify the PC LUTs version");
    Command & cResetCoreLink = registerCommand<ResetCoreLink>("Core link reset");
    Command & cPtLutClockReset = registerCommand<ResetPtLut>("Reset Pt LUT clock");
    Command & cReboot = registerCommand<Reboot>("Reconfigure main FPGA");

    CommandSequence &cfgSeq = registerSequence("Configure Sequence",cVerifyPtLut).
                                                                then(cVerifyPcLutsVersion).
                                                                then(cVerifyPcLuts).
                                                                then(cDaqModuleRst).
                                                                then(cSetDaqCfgRegs).
                                                                then(cSetBC0AndDataDelay).
                                                                then(cSetSingleHits).
                                                                then(cDaqReportWoTrack);

    CommandSequence &ptLutSeq = registerSequence("Load and Verify pT LUT", cResetCoreLink).
                                                                then(cPtLutClockReset).
                                                                then(cWritePtLut).
                                                                then(cVerifyPtLut);


    // processor run control state machine
    swatch::processor::RunControlFSM &pFSM = getRunControlFSM();
    pFSM.coldReset.add(cReboot);
    // pFSM.setup.add(cCheckFWVersion); // TODO: when we enable that we'll need a new DB key
    pFSM.configure.add(cfgSeq);
    // pFSM.align.add(cGthModuleReset);

    cout << "deviceIndex: " << deviceIndex() << " endcap: " << endcap() << " sector: " << sector() << endl;


}

Mtf7Processor::~Mtf7Processor()
{
    delete driver_;
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
    setMetricValue<bool>    (ext_pll_lock_status,    readPLLstatus());
    setMetricValue<int>     (bc0_period_counter,     readBC0counter());
    setMetricValue<double>  (output_track_rate,      readTrackRate());
}

} // namespace

