#include "emtf/ts/cell/EmtfAlgoInterface.hpp"

namespace emtf {

EmtfAlgoInterface::EmtfAlgoInterface(HAL::PCIExprDevice& driver) : driver_(driver)
{
}

EmtfAlgoInterface::~EmtfAlgoInterface()
{
}

void EmtfAlgoInterface::retrieveMetricValues()
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
}

} // namespace

