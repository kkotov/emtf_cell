#include "xdata/Integer.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "xdata/UnsignedInteger32.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/Boolean.h"
#include <iomanip>
#include <cstdlib>
#include "emtf/ts/cell/DAQConfigRegisters.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

DAQConfigRegisters::DAQConfigRegisters(const std::string& aId, ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("BX_offset", xdata::UnsignedInteger(3540u));
    registerParameter("Valor_window", xdata::UnsignedInteger(7u));
    registerParameter("Valor_delay", xdata::UnsignedInteger(50u));
    registerParameter("L1a_window", xdata::UnsignedInteger(7u));
    registerParameter("L1a_delay", xdata::UnsignedInteger(57u));
    registerParameter("Stress", xdata::Boolean(false));
    registerParameter("Amc13_easy_en", xdata::Boolean(false));
    registerParameter("daq_rpc_late_by", xdata::UnsignedInteger(6u));
}

Command::State DAQConfigRegisters::code(const XParameterSet& params)
{
    setStatusMsg("DAQ Set Config Registers started.");

    const uint64_t bxOffset(params.get<xdata::UnsignedInteger>("BX_offset").value_);
    const uint64_t valorWindow(params.get<xdata::UnsignedInteger>("Valor_window").value_);
    const uint64_t valorDelay(params.get<xdata::UnsignedInteger>("Valor_delay").value_);
    const uint64_t l1aWindow(params.get<xdata::UnsignedInteger>("L1a_window").value_);
    const uint64_t l1aDelay(params.get<xdata::UnsignedInteger>("L1a_delay").value_);
    const uint64_t stress(params.get<xdata::Boolean>("Stress").value_);
    const uint64_t amc13EasyEn(params.get<xdata::Boolean>("Amc13_easy_en").value_);
    const uint64_t daqRpcLateBy(params.get<xdata::UnsignedInteger>("daq_rpc_late_by").value_);

    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    Command::State commandStatus = ActionSnapshot::kDone;
    processor.write64("daq_l1a_del", l1aDelay);
    verify::CheckWrittenValue(processor, "daq_l1a_del", l1aDelay, commandStatus);

    processor.write64("daq_l1a_window", l1aWindow);
    verify::CheckWrittenValue(processor, "daq_l1a_window", l1aWindow, commandStatus);

    processor.write64("daq_valor_delay", valorDelay);
    verify::CheckWrittenValue(processor, "daq_valor_delay", valorDelay, commandStatus);

    processor.write64("daq_valor_window", valorWindow);
    verify::CheckWrittenValue(processor, "daq_valor_window", valorWindow, commandStatus);

    processor.write64("daq_bxn_offset", bxOffset);
    verify::CheckWrittenValue(processor, "daq_bxn_offset", bxOffset, commandStatus);

    const uint64_t boardId = processor.sector() | (processor.endcap() << 3); // form board id from the sector and the endcap
    processor.write64("daq_brd_id", boardId);
    verify::CheckWrittenValue(processor, "daq_brd_id", boardId, commandStatus);

    processor.write64("daq_stress_en", stress);
    verify::CheckWrittenValue(processor, "daq_stress_en", stress, commandStatus);

    processor.write64("daq_amc13_easy_en", amc13EasyEn);
    verify::CheckWrittenValue(processor, "daq_amc13_easy_en", amc13EasyEn, commandStatus);

    processor.write64("daq_rpc_late_by", daqRpcLateBy);
    verify::CheckWrittenValue(processor, "daq_rpc_late_by", daqRpcLateBy, commandStatus);

    return commandStatus;
}

} // namespace

