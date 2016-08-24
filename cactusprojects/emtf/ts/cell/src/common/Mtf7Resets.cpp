#include "xdata/Integer.h"
#include "xdata/UnsignedInteger32.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "emtf/ts/cell/Mtf7Resets.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {


DAQModuleReset::DAQModuleReset(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(10000)
{ }

swatch::core::Command::State DAQModuleReset::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Reset the DAQ module.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    Command::State commandStatus = ActionSnapshot::kDone;

    /* TODO: this code performs gth reset and currently it is removed from here
    uint64_t gth_reset = 1u;
    processor.write64("gth_rst", gth_reset);
    verify::CheckWrittenValue(processor, "gth_rst", gth_reset, commandStatus);

    setProgress(0.33);
    usleep(sleepInterval);

    gth_reset = 0u;
    processor.write64("gth_rst", gth_reset);
    verify::CheckWrittenValue(processor, "gth_rst", gth_reset, commandStatus);

    setProgress(0.5);
    usleep(sleepInterval);
    */

    // Substract 1 from the endcap number and the sector number, because in the firmware they start from 0.
    const uint64_t endcap = processor.endcap() - 1;
    processor.write64("endcap", endcap); // add the endcap code
    verify::CheckWrittenValue(processor, "endcap", endcap, commandStatus);

    const uint64_t sector = processor.sector() - 1;
    processor.write64("sector", sector); // add sector code
    verify::CheckWrittenValue(processor, "sector", sector, commandStatus);

    setProgress(1.);

    return commandStatus;
}

// TODO: implement this and use it at power reset
ResetCoreLink::ResetCoreLink(const string& aId, ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(10000)
{ }

swatch::core::Command::State ResetCoreLink::code(const XParameterSet& params)
{
//    setStatusMsg("Reset the core link of the mtf7 board.");
//    Mtf7Processor &processor = getActionable<Mtf7Processor>();
//
//    // write and verify the corelink_axi_reset register
//    uint32_t value = 0u;
//    processor.write("corelink_axi_reset", value);
//    Command::State commandStatus = ActionSnapshot::kDone;
//    verify::CheckWrittenValue(processor, "corelink_axi_reset", value, commandStatus);
//
//    setProgress(0.33);
//
//    usleep(sleepInterval);
//
//    // write and verify the corelink_axi_reset register again
//    value = 1u;
//    processor.write("corelink_axi_reset", value);
//    verify::CheckWrittenValue(processor, "corelink_axi_reset", value, commandStatus);
//
//    setProgress(0.5);
//
//    usleep(sleepInterval);
//
//    // read the value of "corelogic_spy_memory_configuration"
//    uint64_t qvalue = 0;
//
//    processor.read64reg("corelogic_spy_memory_configuration", qvalue);
//
//    qvalue &= 0xfffffffffffffff0ULL; // clear bits for endcap/sector
//    // Substract 1 from the endcap number and the sector number, because in the firmware they start from 0.
//    qvalue |= ((processor.endcap() - 1) & 1); // add the endcap code.
//    qvalue |= (((processor.sector() - 1) & 7) << 1); // add sector code.
//
//    // write and verify the new value of corelogic_spy_memory_configuration back to the board
//    processor.write64reg("corelogic_spy_memory_configuration", qvalue);
//
//    const uint64_t mask = (0x1FFull << 21); // mask for bits 21-29 includive, because they are read only
//    // verify:CheckWrittenValue(processor, "corelogic_spy_memory_configuration", qvalue, commandStatus, mask);
//
//    setProgress(1.);

//    return commandStatus;

    return ActionSnapshot::kError;
}


ResetGthTransceivers::ResetGthTransceivers(const string& aId, ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(10000)
{ }

Command::State ResetGthTransceivers::code(const XParameterSet& params)
{
    setStatusMsg("Reset the GTH module.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    uint64_t gth_reset = 1u;
    processor.write64("gth_rst", gth_reset);

    Command::State commandStatus = ActionSnapshot::kDone;
    verify::CheckWrittenValue(processor, "gth_rst", gth_reset, commandStatus);

    usleep(sleepInterval);
    setProgress(0.5);

    gth_reset = 0u;
    processor.write64("gth_rst", gth_reset);
    verify::CheckWrittenValue(processor, "gth_rst", gth_reset, commandStatus);

    usleep(sleepInterval);
    setProgress(1.0);

    return commandStatus;
}


ResetPtLut::ResetPtLut(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(10000)
{
    // Settings for Vivado-compiled fw
    clkWord.push_back(0x80000100ULL);
    clkWord.push_back(0x00070151ULL); // channel 1 del code 0
    clkWord.push_back(0x00070152ULL); // channel 1 del code 0
    clkWord.push_back(0x00070153ULL); // channel 1 del code 0
    clkWord.push_back(0x10070150ULL); // reg 0 moved here so reg # changes
    clkWord.push_back(0x00020104ULL);
    clkWord.push_back(0x00030105ULL);
    clkWord.push_back(0x00030106ULL);
    clkWord.push_back(0x00020107ULL);
    clkWord.push_back(0x10000908ULL);
    clkWord.push_back(0xA0022A09ULL);
    clkWord.push_back(0x0082000BULL);
    clkWord.push_back(0x0284C00DULL);
    clkWord.push_back(0x0830080EULL);
    clkWord.push_back(0xCC00100FULL);
    clkWord.push_back(0x18070150ULL); // channel 1 del code 0
    clkWord.push_back(0xCC00080FULL);
}

Command::State ResetPtLut::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("This command resets the PT LUT module clock manager.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();
    Command::State commandStatus = ActionSnapshot::kDone;

    uint64_t value;

    for(vector<uint32_t>::const_iterator it = clkWord.begin(); it != clkWord.end(); ++it)
    {
        value = (*it);
        processor.write64reg("ptlut_clk_reset", value);
        usleep(sleepInterval);
        verify::CheckWrittenValue(processor, "ptlut_clk_reset", value, commandStatus);
    }

    return commandStatus;
}


ResetRXBuffers::ResetRXBuffers(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{ }

Command::State ResetRXBuffers::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("This command resets the RX buffers in all GTHs.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    uint64_t value;

    value = (1 << 15); // set the reset bit
    processor.write64reg("corelogic_control", value);

    Command::State commandStatus = ActionSnapshot::kDone;
    verify::CheckWrittenValue(processor, "corelogic_control", value, commandStatus);

    value = 0; // remove the reset bit
    processor.write64reg("corelogic_control", value);
    verify::CheckWrittenValue(processor, "corelogic_control", value, commandStatus);

    return commandStatus;
}


SyncClockReset::SyncClockReset(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    sleepInterval(10000)
{
    syncClkWord.push_back(0x80160140);
    syncClkWord.push_back(0x001F001F); // dummy, just to make lower 4 bits change
    syncClkWord.push_back(0x00140080);
    syncClkWord.push_back(0x00140081);
    syncClkWord.push_back(0x00140082);
    syncClkWord.push_back(0x00140083);
    syncClkWord.push_back(0x00140084);
    syncClkWord.push_back(0x00140805);
    syncClkWord.push_back(0x11110006);
    syncClkWord.push_back(0x10110007);
    syncClkWord.push_back(0x11110008);
    syncClkWord.push_back(0x55555549);
    syncClkWord.push_back(0x91F249AA);
    syncClkWord.push_back(0x1400100B);
    syncClkWord.push_back(0x1B0C006C);
    syncClkWord.push_back(0x2303806D);
    syncClkWord.push_back(0x0300000E);
    syncClkWord.push_back(0x8000800F);
    syncClkWord.push_back(0xC1550410);
    syncClkWord.push_back(0x00000058);
    syncClkWord.push_back(0x02C9C419);
    syncClkWord.push_back(0xAFA8001A);
    syncClkWord.push_back(0x1400321B);
    syncClkWord.push_back(0x0020321C);
    syncClkWord.push_back(0x0180021D);
    syncClkWord.push_back(0x0200021E);
    syncClkWord.push_back(0x001F001F);
}

Command::State SyncClockReset::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("This command performs Sync clock Reset.");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    Command::State commandStatus = ActionSnapshot::kDone;

    for(vector<uint32_t>::const_iterator it = syncClkWord.begin(); it != syncClkWord.end(); ++it)
    {
        processor.write("test_reg1", (*it));
        usleep(sleepInterval);
        verify::CheckWrittenValue(processor, "test_reg1", (*it), commandStatus);
    }

    return commandStatus;
}

} // namespace

