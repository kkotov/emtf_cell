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

#include <fstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "swatch/core/StateMachine.hpp"
#include "swatch/core/MetricConditions.hpp"

using namespace std;
using namespace swatch::core;
using namespace boost::gregorian;
using namespace boost::posix_time;

namespace emtf {

SWATCH_REGISTER_CLASS(emtf::Mtf7Processor);

Mtf7Processor::Mtf7Processor(const swatch::core::AbstractStub& aStub) :
    ext_pll_lock_status(registerMetric<bool>     ("Ext pll lock status", swatch::core::NotEqualCondition<bool>(true), swatch::core::NotEqualCondition<bool>(true))),
    bc0_period_counter (registerMetric<int>      ("Bc0 period counter", swatch::core::NotEqualCondition<int>(3563),  swatch::core::NotEqualCondition<int>(3563))),
    output_track_rate  (registerMetric<double>   ("Output track rate (Hz)")),
    controlFirmwareVersion(registerMetric<string>("Control Firmware Version Timestamp")),
    coreFirmwareVersion(registerMetric<string>   ("Core Firmware Version Timestamp")),
    swatch::processor::Processor(aStub),
    addressTableReader(NULL),
    addressTable(NULL),
    driver_(NULL)
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
    Command & cVerifyPcLuts = registerCommand<VerifyPcLuts>("Verify the PC LUTs");
    Command & cVerifyPcLutsVersion = registerCommand<VerifyPcLutsVersion>("Verify the PC LUTs version");

    CommandSequence &cfgSeq = registerSequence("Configure Sequence", cVerifyPcLutsVersion).
                                                                then(cVerifyPcLuts).
                                                                then(cDaqModuleRst).
                                                                then(cSetDaqCfgRegs).
                                                                then(cSetBC0AndDataDelay).
                                                                then(cSetSingleHits).
                                                                then(cDaqReportWoTrack);

    // processor run control state machine
    swatch::processor::RunControlFSM &pFSM = getRunControlFSM();
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

bool Mtf7Processor::readPLLstatus(void) {
    uint32_t ext_pll_lock = 0u;
    read("ext_pll_lock", ext_pll_lock);
    return ext_pll_lock;
}

int Mtf7Processor::readBC0counter(void) {
    uint32_t bc0_period_cnt = 0u;
    read("bc0_period_cnt", bc0_period_cnt);
    return bc0_period_cnt;
}

double Mtf7Processor::readTrackRate(void) {
    uint64_t output_track_counter = 0u;
    read64("output_track_rate", output_track_counter);
    return (output_track_counter/0.00327);
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


    // TODO: debug code
    // const string fileName("/nfshome0/emtfts/cactus_1701/cactusprojects/emtf/ts/cell/alignment/" + getCrateId() + "_" + boost::lexical_cast<string>(getSlot()));

    // ofstream alignLog(fileName.c_str(), ios::app);

    // if(alignLog.is_open())
    // {
    //     const int size = 49;
    //     const string links[size] = {"me1a_02", "me1a_03", "me1a_04", "me1a_05", "me1a_06", "me1a_07", "me1a_08", "me1a_09",
    //                                 "me1b_02", "me1b_03", "me1b_04", "me1b_05", "me1b_06", "me1b_07", "me1b_08", "me1b_09",
    //                                 "me2_02", "me2_03", "me2_04", "me2_05", "me2_06", "me2_07", "me2_08", "me2_09",
    //                                 "me3_02", "me3_03", "me3_04", "me3_05", "me3_06", "me3_07", "me3_08", "me3_09",
    //                                 "me4_02", "me4_03", "me4_04", "me4_05", "me4_06", "me4_07", "me4_08", "me4_09",
    //                                 "me1n_03", "me1n_06", "me1n_09",
    //                                 "me2n_03", "me2n_09",
    //                                 "me3n_03", "me3n_09",
    //                                 "me4n_03", "me4n_09" };

    //     string afDelayStr;
    //     for(int i=0; i<size; ++i)
    //     {
    //         const string regName("af_delay_" + links[i]);

    //         uint64_t afDelay;

    //         driver_->read64(regName, &afDelay);

    //         afDelayStr.append(boost::lexical_cast<string>(afDelay) + "  ");
    //     }
    //     afDelayStr.append("\n");

    //     alignLog << afDelayStr;

    //     alignLog.close();
    // }
}

} // namespace

