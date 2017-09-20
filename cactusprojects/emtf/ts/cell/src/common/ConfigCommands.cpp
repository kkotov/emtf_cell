#include "xdata/String.h"
#include "xdata/Integer.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "emtf/ts/cell/ConfigCommands.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

CheckFWVersion::CheckFWVersion(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("control_firmware_version", xdata::String("yyyy-mm-dd hh:mm:ss"));
    registerParameter("core_firmware_version", xdata::String("YYYY-MM-DD HH:MM:SS"));
}

swatch::action::Command::State CheckFWVersion::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Compare the firmware version of the board with the one from the DB.");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const xdata::String controlFwVersion(params.get<xdata::String>("control_firmware_version").value_);
    const xdata::String coreFwVersion(params.get<xdata::String>("core_firmware_version").value_);

    const string controlFwVersionBoard = processor.readControlFirmwareVersion();
    const string coreFwVersionBoard = processor.readCoreFirmwareVersion();

    Command::State commandStatus = Functionoid::kDone;

    if(   (controlFwVersion.toString() != controlFwVersionBoard)
       || (coreFwVersion.toString() != coreFwVersionBoard))
    {

        stringstream msg1, msg2;

        if( controlFwVersion.toString() != controlFwVersionBoard )
        {
            msg1 << " expected control FW ";
            msg1 << controlFwVersion.toString();
            msg1 << " does not match found ";
            msg1 << controlFwVersionBoard;
        }

        if( coreFwVersion.toString() != coreFwVersionBoard )
        {
            msg2 << "  expected core FW ";
            msg2 << coreFwVersion.toString();
            msg2 << " does not match found ";
            msg2 << coreFwVersionBoard;
        }

        setStatusMsg("Compare the firmware version fails: " + msg1.str() + msg2.str());

        commandStatus = Functionoid::kError;
    }

    setProgress(1.);

    return commandStatus;
}

AlgoConfig::AlgoConfig(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("th_window",      xdata::UnsignedInteger(8u));
    registerParameter("low_th_promote", xdata::UnsignedInteger(0u));
}

swatch::action::Command::State AlgoConfig::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Setting track-finding algorithm parameters");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    Command::State commandStatus = Functionoid::kDone;

    const uint32_t th_window(params.get<xdata::UnsignedInteger>("th_window").value_);
    processor.write("th_window", th_window);
    verify::CheckWrittenValue(processor, "th_window", th_window, commandStatus);

    const uint32_t low_th_promote(params.get<xdata::UnsignedInteger>("low_th_promote").value_);
    processor.write("low_th_promote", low_th_promote);
    verify::CheckWrittenValue(processor, "low_th_promote", low_th_promote, commandStatus);

    setProgress(1.);

    return commandStatus;
}

SetDoubleMuonTrg::SetDoubleMuonTrg(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::UnsignedInteger(0u))
{
    registerParameter("two_muons_trigger_enabled", xdata::UnsignedInteger(0u));
    registerParameter("delay_two_muons_trigger",   xdata::UnsignedInteger(9u));
}

swatch::action::Command::State SetDoubleMuonTrg::code(const XParameterSet& params)
{
    setStatusMsg("Enable the single hit algorithm.");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    const uint32_t diMuTrig(params.get<xdata::UnsignedInteger>("two_muons_trigger_enabled").value_);
    const uint32_t diMuDelay(params.get<xdata::UnsignedInteger>("delay_two_muons_trigger").value_);

    Command::State commandStatus = Functionoid::kDone;

    processor.write("two_mu_en", diMuTrig);
    verify::CheckWrittenValue(processor, "two_mu_en", diMuTrig, commandStatus);

    processor.write("delay_two_mu", diMuDelay);
    verify::CheckWrittenValue(processor, "delay_two_mu", diMuDelay, commandStatus);

    return commandStatus;
}



} // namespace

