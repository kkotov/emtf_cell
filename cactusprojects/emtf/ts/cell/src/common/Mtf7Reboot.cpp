#include "xdata/Integer.h"
#include "xdata/UnsignedInteger32.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "emtf/ts/cell/Mtf7Reboot.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

Reboot::Reboot(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))//,
//    sleepInterval(200000)
{
}

Command::State Reboot::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Rebooting/Reconfiguring main FPGA");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    Command::State commandStatus = ActionSnapshot::kDone;

    uint32_t val = 0;
    processor.read("flash_core_fw_start",val);
std::cout<<"core    f/w start: 0x"<<std::hex<<val<<std::dec<<" (hardcoded)"<<std::endl;
    processor.read("flash_cnt_fw_start", val);
std::cout<<"control f/w start: 0x"<<std::hex<<val<<std::dec<<" (hardcoded)"<<std::endl;

    processor.write("flash_core_config_start", 0x1);

    for (int j = 0; j < 20; j++)
    {
        uint32_t core_done = 0;
        processor.read("flash_core_done",val);

//std::cout<<"core: OK: " << ((val >>3)&1) << " d: "<< ((val >>2)&1) <<" i: "<< ((val >>1)&1) <<" e: "<< (val&1) << "   " << std::endl;

        processor.read("flash_core_config_sm_state", val);
std::cout<<"sm state: core: "<< (val & 0xf) << std::endl;
        if (core_done) break; // core done is up
        usleep(200000);
    }

    return commandStatus;
}

} // namespace

/*
device index: 1
core    f/w start: 190000 (hardcoded)
control f/w start: 0 (hardcoded)
core: OK: 0 d: 0 i: 0 e: 1   sm state: core: 3
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 4
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 5
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 4
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 4
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 4
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 5
core: OK: 0 d: 0 i: 1 e: 1   sm state: core: 4
core: OK: 1 d: 1 i: 1 e: 0   sm state: core: 0


core    f/w start: 0x190000 (hardcoded)
control f/w start: 0x0 (hardcoded)
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 3
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 6
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 1  sm state: core: 4
core: OK: 0 d: 0 i: 0 e: 0  sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0
core: OK: 0 d: 0 i: 0 e: 0   
sm state: core: 0

*/
