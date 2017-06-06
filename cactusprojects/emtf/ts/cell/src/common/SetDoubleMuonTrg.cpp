#include "xdata/Integer.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "xdata/UnsignedInteger.h"
#include "xdata/UnsignedInteger64.h"
#include "xdata/Boolean.h"
#include <iomanip>
#include <cstdlib>
#include "emtf/ts/cell/SetDoubleMuonTrg.hpp"

using namespace std;
using namespace swatch::core;
using namespace swatch::action;


namespace emtf {

SetDoubleMuonTrg::SetDoubleMuonTrg(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("two_muons_trigger_enabled", xdata::UnsignedInteger(0u));
    registerParameter("delay_two_muons_trigger",   xdata::UnsignedInteger(9u));
}

Command::State SetDoubleMuonTrg::code(const XParameterSet& params)
{
    setStatusMsg("Enable the single hit algorithm.");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const uint64_t diMuTrig(params.get<xdata::UnsignedInteger>("two_muons_trigger_enabled").value_);
    const uint64_t diMuDelay(params.get<xdata::UnsignedInteger>("delay_two_muons_trigger").value_);

    Command::State commandStatus = Functionoid::kDone;

    processor.write64("two_mu_en", diMuTrig);
    verify::CheckWrittenValue(processor, "two_mu_en", diMuTrig, commandStatus);

    processor.write64("delay_two_mu", diMuDelay);
    verify::CheckWrittenValue(processor, "delay_two_mu", diMuDelay, commandStatus);

    return commandStatus;
}

} // namespace

