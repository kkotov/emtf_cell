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
        commandStatus = Functionoid::kError;
    }

    setProgress(1.);

    return commandStatus;
}

} // namespace

