#include "emtf/ts/cell/EmtfTTCInterface.hpp"

namespace emtf {

Mtf7TTCInterface::Mtf7TTCInterface(HAL::PCIExprDevice& driver) : board(driver)
{
}

Mtf7TTCInterface::~Mtf7TTCInterface()
{
}

void Mtf7TTCInterface::retrieveMetricValues()
{
    setMetricValue<uint32_t>(mMetricBunchCounter, readBunchCounter());
    setMetricValue<uint32_t>(mMetricL1ACounter, readL1ACounter());
    setMetricValue<uint32_t>(mMetricOrbitCounter, readOrbitCounter());

    setMetricValue<bool>(mMetricIsClock40Locked, readIsClock40Locked());

    setMetricValue<bool>(mMetricHasClock40Stopped, readHasClock40Stopped());

    setMetricValue<bool>(mMetricIsBC0Locked, readIsBC0Locked());

    setMetricValue<uint32_t>(mMetricSingleBitErrors, readSingleBitErrors());

    setMetricValue<uint32_t>(mMetricDoubleBitErrors, readDoubleBitErrors());
}

uint32_t Mtf7TTCInterface::readBunchCounter()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

uint32_t Mtf7TTCInterface::readL1ACounter()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

uint32_t Mtf7TTCInterface::readOrbitCounter()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

bool Mtf7TTCInterface::readIsClock40Locked()
{
    uint32_t extPllLock = 0;

    board.read("ext_pll_lock", &extPllLock);

    return (1 == extPllLock);
}

bool Mtf7TTCInterface::readHasClock40Stopped()
{
    uint32_t extPllLock = 1;

    board.read("ext_pll_lock", &extPllLock);

    return (0 == extPllLock);
}

bool Mtf7TTCInterface::readIsBC0Locked()
{
    uint32_t bc0PeriodCnt = 0;

    board.read("bc0_period_cnt", &bc0PeriodCnt);

    return (3563 == bc0PeriodCnt);
}

uint32_t Mtf7TTCInterface::readSingleBitErrors()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

uint32_t Mtf7TTCInterface::readDoubleBitErrors()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

} // namespace

