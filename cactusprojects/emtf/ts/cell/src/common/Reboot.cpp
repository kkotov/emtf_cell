#include <sstream>
#include "xdata/Integer.h"
#include "xdata/UnsignedInteger32.h"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "emtf/ts/cell/Reboot.hpp"

using namespace std;
using namespace swatch::core;
using namespace swatch::action;
using namespace log4cplus;

namespace emtf {

Reboot::Reboot(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(200000)
{
}

void Reboot::log(const char *prefix, uint32_t val, const char *suffix)
{
    std::stringstream oss;
    oss << prefix << std::hex << val << std::dec << suffix;
    log4cplus::Logger generalLogger( log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()) );
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT( oss.str()));
//    setStatusMsg(oss.str());
}

Command::State Reboot::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Rebooting/Reconfiguring main FPGA");
    setProgress(1.);

    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    Command::State commandStatus = Functionoid::kDone;

    uint32_t val = 0;
    processor.read("flash_core_fw_start",val);
    log("core    f/w start: 0x",val);

    processor.read("flash_cnt_fw_start", val);
    log("control f/w start: 0x",val);

    processor.write("flash_core_config_start", 0x1);

    for (int j = 0; j < 20; j++)
    {
        uint32_t core_done = 0;
        processor.read("flash_core_done",core_done);
        processor.read("flash_core_config_sm_state", val);
        log("sm state: core: ", val & 0xf);

        if( core_done ) break; // core done is up
        usleep( sleepInterval );
    }

    setProgress(1.);

    return commandStatus;
}

} // namespace

