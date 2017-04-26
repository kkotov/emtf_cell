#include "emtf/ts/cell/EmtfReadoutInterface.hpp"

using namespace swatch::core;

namespace emtf {

Mtf7ReadoutInterface::Mtf7ReadoutInterface(HAL::PCIExprDevice& driver) :
    board(driver)
{
}

void Mtf7ReadoutInterface::retrieveMetricValues()
{
    // Read bunch, orbit & event counters from hardware
    // Note, of course, the following three lines could be very different for different subystems, depending on how they read these values from their hardware, and the API of their driver classes.
    // uint32_t bunchCounterValue = driver_.readBunchCounter();
    // uint32_t orbitCounterValue = driver_.readOrbitCounter();
    // uint32_t eventCounterValue = driver_.readEventCounter();

    // Plug these values into the SWATCH metric framework
    // setMetricValue<>(metricBunchCounter_, bunchCounterValue);
    // setMetricValue<>(metricOrbitCounter_, orbitCounterValue);
    // setMetricValue<>(metricEventCounter_, eventCounterValue);

    // N.B: This method should also set the values of 5 other common metrics that are defined in the swatch::processor::TTCInterface class; they have been missed out here to keep the example brief

    setMetricValue<tts::State>(mMetricTTS, readMetricTTS());
    setMetricValue<bool>(mMetricAMCCoreReady, readMetricAMCCoreReady());
    setMetricValue<uint32_t>(mMetricEventCounter, readMetricEventCounter());
}

tts::State Mtf7ReadoutInterface::readMetricTTS()
{
    uint64_t daqTtsState = 0;
    board.read64("daq_tts_state", &daqTtsState);

    if(   tts::kWarning      == daqTtsState
       || tts::kOutOfSync    == daqTtsState
       || tts::kBusy         == daqTtsState
       || tts::kReady        == daqTtsState
       || tts::kError        == daqTtsState
       || tts::kDisconnected == daqTtsState)
    {
        return static_cast<tts::State>(daqTtsState);
    }

    return tts::kUnknown;
}

bool Mtf7ReadoutInterface::readMetricAMCCoreReady()
{
    uint64_t daqTtsState = 0;
    board.read64("daq_tts_state", &daqTtsState);

    return (tts::kReady == daqTtsState);
}

uint32_t Mtf7ReadoutInterface::readMetricEventCounter()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

} // namespace

