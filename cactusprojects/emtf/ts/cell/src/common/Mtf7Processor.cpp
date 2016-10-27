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
#include "emtf/ts/cell/TransitionCommands.hpp"
#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/format.hpp>

#include "swatch/core/StateMachine.hpp"
#include "swatch/core/MetricConditions.hpp"


using namespace std;
using namespace swatch::core;
using namespace swatch::processor;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace log4cplus;

namespace emtf {


static bool filterOutMaskedPorts(const swatch::core::MonitorableObject& object)
{
    return !dynamic_cast<const InputPort&>(object).isMasked();
}

static const uint16_t* countObjectsInError(const std::vector<swatch::core::MonitorableObjectSnapshot>& objects)
{
    uint16_t count = 0;

    for(auto it=objects.begin(); it!=objects.end(); ++it)
    {
        if(it->getStatusFlag() != kGood)
        {
            ++count;
        }
    }

    return new uint16_t(count);
}


SWATCH_REGISTER_CLASS(emtf::Mtf7Processor);

Mtf7Processor::Mtf7Processor(const swatch::core::AbstractStub& aStub) :
    extPllLockStatus(registerMetric<bool>("extPllLockStatus",
                                             NotEqualCondition<bool>(true),
                                             NotEqualCondition<bool>(true))),
    bc0PeriodCounter(registerMetric<int>("bc0PeriodCounter",
                                             NotEqualCondition<int>(3563),
                                             NotEqualCondition<int>(3563))),
    outputTrack0Rate(registerMetric<uint32_t>("outputTrack0Rate")),
    outputTrack1Rate(registerMetric<uint32_t>("outputTrack1Rate")),
    outputTrack2Rate(registerMetric<uint32_t>("outputTrack2Rate")),
    controlFirmwareVersion(registerMetric<string>("controlFwVersionTimestamp")),
    coreFirmwareVersion(registerMetric<string>   ("coreFwaVersionTimestamp")),
    swatch::processor::Processor(aStub),
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


    vector<MonitorableObject*> inputPorts;
    for(auto it=getInputPorts().getPorts().begin(); it!=getInputPorts().getPorts().end(); ++it)
    {
        inputPorts.push_back(*it);
    }
    ComplexMetric<uint16_t> & brokenLinks = registerComplexMetric<uint16_t>("numberOfBrokenInputLinks",
                                                                            inputPorts.begin(),
                                                                            inputPorts.end(),
                                                                            ComplexMetric<uint16_t>::CalculateFunction2_t(&countObjectsInError),
                                                                            &filterOutMaskedPorts);
    setErrorCondition(brokenLinks, GreaterThanCondition<uint16_t>(config::brokenLinksErrorProcessor()));
    setWarningCondition(brokenLinks, RangeCondition<uint16_t>(config::brokenLinksWarningProcessor(),
                                                              config::brokenLinksErrorProcessor()));

    generateLctPairs();


    Command & cDaqModuleRst = registerCommand<DAQModuleReset>("DAQ Module Reset");
    // Command & cGthModuleReset = registerCommand<ResetGthTransceivers>("GTH Module Reset");
    Command & cSetDaqCfgRegs = registerCommand<Mtf7DAQConfigRegisters>("Set DAQ Config Registers");
    Command & cSetBC0AndDataDelay = registerCommand<Mtf7SetDelaysAndTriggerSource>("Set BC0 and Data Delay");
    Command & cSetSingleHits = registerCommand<Mtf7SetSingleHits>("Enable the single hit algorithm");
    Command & cDaqReportWoTrack = registerCommand<Mtf7DaqReportWoTrack>("Enable the firmware report in DAQ stream");
    // Command & cCheckFWVersion = registerCommand<CheckFWVersion>("Compare the firmware version");

    // Command & cWritePcLuts = registerCommand<WritePcLuts>("Write the PC LUTs to the board");
    Command & cVerifyPcLuts = registerCommand<VerifyPcLuts>("Verify the PC LUTs");
    Command & cVerifyPcLutsVersion = registerCommand<VerifyPcLutsVersion>("Verify the PC LUTs version");

    Command & cVerifyPtLut = registerCommand<VerifyPtLut>("Verify the Pt LUT on the board");
    Command & cWritePtLut = registerCommand<WritePtLut>("Write the Pt LUT to the board");
    Command & cVerifyWritePtLut = registerCommand<VerifyWritePtLut>("Verify and Write the Pt LUT on the board");
    Command & cVerifyPtLutVersion = registerCommand<VerifyPtLutVersion>("Verify the Pt LUT version");

    Command & cOnStart = registerCommand<OnStart>("Executed at the transition from 'Aligned' to 'Running'");
    Command & cResetCoreLink = registerCommand<ResetCoreLink>("Core link reset");
    Command & cPtLutClockReset = registerCommand<ResetPtLut>("Reset Pt LUT clock");
    Command & cReboot = registerCommand<Reboot>("Reconfigure main FPGA");

    CommandSequence &cfgSeq = registerSequence("Configure Sequence", cDaqModuleRst).
                                                                then(cSetDaqCfgRegs).
                                                                then(cSetBC0AndDataDelay).
                                                                then(cSetSingleHits).
                                                                then(cDaqReportWoTrack).
                                                                then(cVerifyPcLutsVersion).
                                                                then(cVerifyPcLuts).
                                                                then(cVerifyPtLutVersion).
                                                                then(cVerifyWritePtLut);



    CommandSequence &ptLutSeq = registerSequence("Load and Verify Pt LUT", cResetCoreLink).
                                                                then(cPtLutClockReset).
                                                                then(cVerifyWritePtLut);


    // processor run control state machine
    RunControlFSM &pFSM = getRunControlFSM();
    // pFSM.coldReset.add(cReboot).add(ptLutSeq);
    // pFSM.setup.add(cCheckFWVersion); // TODO: when we enable that we'll need a new DB key
    // pFSM.configure.add(cfgSeq);
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

uint32_t Mtf7Processor::readTrackRate(uint16_t track)
{
    uint64_t trackCounter = 0u;

    boost::format regNameTemplate("rate_track_%u_1");
    read64((regNameTemplate % track).str(), trackCounter);

    boost::format msgTemplate("%s output track_%u rate: %u Hz");
    const string trackRateMsg((msgTemplate % getStub().id % track % trackCounter).str());

    LOG4CPLUS_TRACE(rateLogger, LOG4CPLUS_TEXT(trackRateMsg));

    return trackCounter;
}

uint32_t Mtf7Processor::lctRate(string lctName)
{
    uint64_t inputLctRate = 0u;

    read64(lctName, inputLctRate);

    return inputLctRate;
}

void Mtf7Processor::generateLctPairs()
{
    vector<string> stationNames = {"me1a", "me1b", "me2", "me3", "me4"};

    boost::format metricTemplate("rateLct_%s_0%u");
    boost::format registerTemplate("rate_lct_%s_0%u");

    string metricName;
    Metric<uint32_t> *metric;

    // generate the metricName-registerName pairs for the stations 1a, 1b, 2, 3 and 4
    for(auto it=stationNames.begin(); it!=stationNames.end(); ++it)
    {
        for(uint32_t i=1; i<=9; ++i)
        {
            metricName = (metricTemplate % (*it) % i).str();
            metric = &registerMetric<uint32_t>(metricName);

            pair<Metric<uint32_t> *, string> pair(metric, (registerTemplate % (*it) % i).str());

            lctRates.push_back(pair);
        }
    }


    // generate the metricName-registerName pairs for the neighbour stations 1n
    for(uint32_t i=3; i<=9; i+=3)
    {
        metricName = (metricTemplate % "me1n" % i).str();
        metric = &registerMetric<uint32_t>(metricName);

        pair<Metric<uint32_t> *, string> pair(metric, (registerTemplate % "me1n" % i).str());

        lctRates.push_back(pair);
    }

    // generate the metricName-registerName pairs for the neighbour stations 2n, 3n and 4n
    vector<string> stationNeighbourNames = {"me2n", "me3n", "me4n"};
    for(auto it=stationNeighbourNames.begin(); it!=stationNeighbourNames.end(); ++it)
    {
        for(uint32_t i=3; i<=9; i+=6)
        {
            metricName = (metricTemplate % (*it) % i).str();
            metric = &registerMetric<uint32_t>(metricName);

            pair<Metric<uint32_t> *, string> pair(metric, (registerTemplate % (*it) % i).str());

            lctRates.push_back(pair);
        }
    }
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
    setMetricValue<bool>    (extPllLockStatus,       readPLLstatus());
    setMetricValue<int>     (bc0PeriodCounter,       readBC0counter());
    setMetricValue<uint32_t>(outputTrack0Rate,       readTrackRate(0));
    setMetricValue<uint32_t>(outputTrack1Rate,       readTrackRate(1));
    setMetricValue<uint32_t>(outputTrack2Rate,       readTrackRate(2));

    // update the metrics for the LCTs
    for(auto it=lctRates.begin(); it!=lctRates.end(); ++it)
    {
        setMetricValue<uint32_t>(*(*it).first, lctRate((*it).second));
    }
}

} // namespace

