#include "xdata/String.h"
#include "xdata/Integer.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

// CheckFWVersion::CheckFWVersion(const std::string& aId, swatch::core::ActionableObject& aActionable) :
//     swatch::core::Command(aId, aActionable, xdata::Integer(0))
// {
//     registerParameter("control_firmware_version", xdata::String("yyyy-mm-dd hh:mm:ss"));
//     registerParameter("core_firmware_version", xdata::String("YYYY-MM-DD HH:MM:SS"));
// }

// swatch::core::Command::State CheckFWVersion::code(const swatch::core::XParameterSet& params)
// {
//     setStatusMsg("Compare the firmware version of the board with the one from the DB.");
//     Mtf7Processor &processor = getActionable<Mtf7Processor>();
// 
//     const xdata::String controlFwVersion(params.get<xdata::String>("control_firmware_version").value_);
//     const xdata::String coreFwVersion(params.get<xdata::String>("core_firmware_version").value_);
// 
//     const string controlFwVersionBoard = processor.readControlFirmwareVersion();
//     const string coreFwVersionBoard = processor.readCoreFirmwareVersion();
// 
//     Command::State commandStatus = ActionSnapshot::kDone;
// 
//     if(   (controlFwVersion.toString() != controlFwVersionBoard)
//        || (coreFwVersion.toString() != coreFwVersionBoard))
//     {
//         commandStatus = ActionSnapshot::kError;
//     }
// 
//     setProgress(1.);
// 
//     return commandStatus;
// }

} // namespace

