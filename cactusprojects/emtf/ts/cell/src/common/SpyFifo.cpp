#include "xdata/Integer.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "xdata/UnsignedInteger.h"
#include "xdata/UnsignedInteger64.h"
#include "xdata/Boolean.h"
#include <iomanip>
#include <cstdlib>
#include "emtf/ts/cell/SpyFifo.hpp"

using namespace std;
using namespace swatch::core;
using namespace swatch::action;


namespace emtf {


SetDelaysAndTriggerSource::SetDelaysAndTriggerSource(const std::string& aId, ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("BC0_offset", xdata::UnsignedInteger(24u));
    registerParameter("DAQ_delay", xdata::Integer(272u));
    registerParameter("GMT_comma_delay", xdata::UnsignedInteger(34u));
}

Command::State SetDelaysAndTriggerSource::code(const XParameterSet& params)
{
    setStatusMsg("Sets the delays and the trigger source");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const uint64_t bc0Offset(params.get<xdata::UnsignedInteger>("BC0_offset").value_);
    const uint64_t daqDelay(params.get<xdata::Integer>("DAQ_delay").value_);
    const uint64_t gmtCommaDelay(params.get<xdata::UnsignedInteger>("GMT_comma_delay").value_);

    Command::State commandStatus = Functionoid::kDone;

    processor.write64("spy_data_delay", daqDelay);
    verify::CheckWrittenValue(processor, "spy_data_delay", daqDelay, commandStatus);

    const uint64_t afEnable = 1u;
    processor.write64("af_en", afEnable);
    verify::CheckWrittenValue(processor, "af_en", afEnable, commandStatus);

    processor.write64("af_ttc_bc0_delay", bc0Offset);
    verify::CheckWrittenValue(processor, "af_ttc_bc0_delay", bc0Offset, commandStatus);

    processor.write64("gmt_link_comma_delay", gmtCommaDelay);
    verify::CheckWrittenValue(processor, "gmt_link_comma_delay", gmtCommaDelay, commandStatus);

    return commandStatus;
}


SetSingleHits::SetSingleHits(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("single_hits_enabled", xdata::Boolean(false));
}

Command::State SetSingleHits::code(const XParameterSet& params)
{
    setStatusMsg("Enable the single hit algorithm.");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const uint64_t singleHits(params.get<xdata::Boolean>("single_hits_enabled").value_);

    Command::State commandStatus = Functionoid::kDone;

    processor.write64("single_en", singleHits);
    verify::CheckWrittenValue(processor, "single_en", singleHits, commandStatus);

    return commandStatus;
}


DaqReportWoTrack::DaqReportWoTrack(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("daq_report_wo_track_enabled", xdata::Boolean(true));
}

Command::State DaqReportWoTrack::code(const XParameterSet& params)
{
    setStatusMsg("Enable the firmware report in DAQ stream for the events that don't have valid tracks in them.");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const uint64_t daqReportWoTrack(params.get<xdata::Boolean>("daq_report_wo_track_enabled").value_);

    Command::State commandStatus = Functionoid::kDone;

    processor.write64("daq_report_wo_track", daqReportWoTrack);
    verify::CheckWrittenValue(processor, "daq_report_wo_track", daqReportWoTrack, commandStatus);

    return commandStatus;
}


} // namespace

