#include "xdata/String.h"
#include "xdata/Integer.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

InitReset::InitReset(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<Mtf7Processor>())
{
}

swatch::core::Command::State InitReset::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Initializing the processor");
    //Mtf7Processor &processor = getActionable<Mtf7Processor>();

    setStatusMsg("Reseting the core links");

    processor.write("core_link_rst",0x0);
    usleep(10000);

    processor.write("core_link_rst",0x1);
    usleep(10000);

    setStatusMsg("Writing endcap and sector");

    processor.write64("endcap", processor.endcap()-1 ); // -1 offset to start from 0: Endcap 1 = URI[0-5] = '+' and Endcap 2 = URI[6-11] = '-'
    processor.write64("sector", processor.sector()-1 ); // -1 offset ...

    Command::State commandStatus = ActionSnapshot::kDone;
    setProgress(1.);
    return commandStatus;
}

CheckFWVersion::CheckFWVersion(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("control_firmware_version", xdata::String("yyyy-mm-dd hh:mm:ss"));
    registerParameter("core_firmware_version", xdata::String("YYYY-MM-DD HH:MM:SS"));
}

swatch::core::Command::State CheckFWVersion::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Compare the firmware version of the board with the one from the DB.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    const xdata::String controlFwVersion(params.get<xdata::String>("control_firmware_version").value_);
    const xdata::String coreFwVersion(params.get<xdata::String>("core_firmware_version").value_);

    const string controlFwVersionBoard = processor.readControlFirmwareVersion();
    const string coreFwVersionBoard = processor.readCoreFirmwareVersion();

    Command::State commandStatus = ActionSnapshot::kDone;

    if(   (controlFwVersion.toString() != controlFwVersionBoard)
       || (coreFwVersion.toString() != coreFwVersionBoard))
    {
        commandStatus = ActionSnapshot::kError;
    }

    setProgress(1.);

    return commandStatus;
}

} // namespace

